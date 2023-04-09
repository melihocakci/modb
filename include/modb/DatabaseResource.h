#ifndef DATABASERESOURCE_H
#define DATABASERESOURC_H

#include <db_cxx.h>

#include <iostream>


namespace modb {
    typedef enum {
        DB_NONE=1,
        DB_OPENED=2, // first state
        DB_OVERWRITTEN=3, // if db there and overwritten it
        DB_FORKED=4, // if database is copied it is state is forked 
        DB_ERROR=5, // if any opening error occurs
        DB_UNKNOWN=6 // others 
    } RESOURCE_STATUS;

    
   
    class DataObject
    {
        // DataObject() = default;

        bool status = true;
    };

    template<typename T>
    struct Serializer
    {
        public:
        Serializer() = default;
        const std::string& Serialize(T);
        const T& Deserialize(std::string&);
    };

    template<typename T>
    class DatabaseResource
    {
        public:
        DatabaseResource() = default;

        DatabaseResource(DatabaseResource& other) = default;
        DatabaseResource(const std::string& dbName, DBTYPE type);
        DatabaseResource(Db* database);
        DatabaseResource& operator=(std::nullptr_t);

        Db* CopyDB(Db* database);
        Serializer<T>& Serializer();


        void SetErrorStream(__DB_STD(ostream) *error_stream);
    
        void Open(DBTYPE);
        void WriteKeyValuePair(const std::string&, const std::string&);
        // BulkLoad();
        // BulkWrite();
        T FindById(const std::string&);
        // UpdateByOid();
        // DeleteByOid();

         ~DatabaseResource() ;

        private:
            void m_ExceptionForOpening();
            void m_SafeModLog(const std::string&);
            void m_InstantiateSerializer();      
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