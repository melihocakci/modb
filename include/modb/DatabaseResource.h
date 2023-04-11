#ifndef DATABASERESOURCE_H
#define DATABASERESOURCE_H

#include <db_cxx.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>


using json = nlohmann::json;


namespace modb {
    typedef enum {
        DB_NONE=1,
        DB_OPENED=2, // first state
        DB_OVERWRITTEN=3, // if db there and overwritten it
        DB_FORKED=4, // if database is copied it is state is forked 
        DB_ERROR=5, // if any opening error occurs
        DB_UNKNOWN=6 // others 
    } RESOURCE_STATUS;

    typedef enum {
        WRITE_DEFAULT=0,
        WRITE_APPEND=2,
        WRITE_NODUPDATA=19,
        WRITE_NOOVERWRITE=20,
        WRITE_WRITEMULTIPLE=2048,
        WRITE_MULTIPLE_KEY=16384,
        WRITE_OVERWRITE_DUP=21,

    } RECORD_WRITE_OPTION;
   
    

    // Purpose of handle exception
    class DataObject
    {
        public:
        // there is generic solution to this 
        // first solution i think give as well as schema data as parameter.   
        virtual bool SetJson(json json){
            std::string data = json["status"];
            if(data.empty()) {
                return false;
            }

            std::string compared = "True";
            status = !data.compare(compared);

            return true;
        }; 

        bool status = true; // faulted data model means false
    };

    template<typename T>
    class Serializer
    {
        public:
        Serializer() = default;
        Serializer(Serializer<T>&) = default;
        std::string& Serialize(T);
        T& Deserialize(std::string&);
        ~Serializer();

        T& GetData();
        std::string& GetSerializedData();

        private:
        std::string m_serializedData;
        T m_data;
    };

    template<typename T>
    class DatabaseResource
    {
        public:
        DatabaseResource();

        DatabaseResource(DatabaseResource& other) = default;
        DatabaseResource(const std::string& dbName, DBTYPE type);
        DatabaseResource(Db* database);
        DatabaseResource& operator=(std::nullptr_t);

        Db* CopyDB(Db* database);
        Serializer<T>& Serializer_();


        void SetErrorStream(__DB_STD(ostream) *error_stream);
    
        void Open(DBTYPE);
        void WriteKeyValuePair(const std::string&, const std::string&, RECORD_WRITE_OPTION);
        // BulkLoad();
        // BulkWrite();
        T& FindById(const std::string&);
        // UpdateByOid();
        // DeleteByOid();
         ~DatabaseResource() = default;

        DB* Database();

        private:
            void m_ExceptionForOpening();
            void m_SafeModLog(const std::string&);
            void m_InstantiateSerializer();
            Dbt* m_ConvertDbt(const std::string&);
            void m_SetDBPoint(Db*);

            Db* m_database; // bdb source, there will be generic class for all DB later
            std::string m_databaseName;
            RESOURCE_STATUS m_status = modb::DB_NONE;
            modb::Serializer<T> m_serializer;
            // there will be used in tracking object state and 
            // Manager use to this as garbage collector
            bool m_isSafe;
            
    };

}


#endif