#ifndef EXAMPLE_H
#define EXAMPLE_H
#include <modb/Point.h>
#include <string>

namespace modb 
{
    class Example {
        public:
            Example() = default;

            ~Example() = default;

            Example(std::string id, modb::Point baseLocation, modb::Point mbrLocation, float mbrWidth);
            template <class Archive>
            inline void serialize(Archive& ar, unsigned int) {
                ar& m_oid;
                ar& m_baseLocation;
                ar& m_mbrLocation;
                ar& m_mbrWidth;
            }
            std::string getOid();

            modb::Point getBaseLocation();

            modb::Point getMbrLocation();

            float getMbrWidth();

        private:
            char * code;
    };

}

#endif



#ifndef DATABASERESOURCE_H
#define DATABASERESOURCE_H

#include <db_cxx.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

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

    
   
    // Purpose of handle exception while continuous data sending
    class DataObject
    {
        public:
        bool status = true; // faulted data model means false
    };

    template<typename T>
    struct Serializer
    {
        public:
        Serializer() = default;
        const inline std::string& Serialize(T data) {
            T object{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, 1.1 };

            std::ostringstream oss{};
            boost::archive::binary_oarchive oa(oss);


            oa << object;
            std::string serialized{oss.str()};

            return serialized;
        }
 
        const T& Deserialize(std::string& serializedData) {
            std::istringstream iss{serializedData};
            boost::archive::binary_iarchive ia{iss};

            T readRecord;

            ia >> readRecord;
            
            return readRecord;
        }
    };

    template<typename T>
    class DatabaseResource
    {
        public:
        DatabaseResource() 
        {
            m_InstantiateSerializer();
        }

        DatabaseResource(DatabaseResource& other) = default;
        DatabaseResource(const std::string& dbName, DBTYPE type) 
        {
            m_InstantiateSerializer();
            try
            {
                Db myDb{ NULL, 0 };
                myDb.set_error_stream(&std::cerr);
                myDb.open(NULL, dbName.c_str(), NULL, type, DB_CREATE, 0);
            }
            catch(const std::exception& e)
            {
                modb::DatabaseResource<T>::m_ExceptionForOpening();
            }
            m_status = modb::DB_OPENED;
            
        }
        void CopyDB(Db* database) {
            m_InstantiateSerializer();
            if(m_status == modb::DB_OPENED) {
                m_SafeModLog("Overwriting existing data resource");
                m_status = modb::DB_OVERWRITTEN;
                
                try {
                    m_database->close(0);
                }
                catch (DbException e) 
                {
                    throw std::runtime_error("While closing database error occured.");
                }

                delete m_database;

            }
            const char ** fname = (const char **) malloc(1);
            (*fname) = (const char*) malloc(100);
            const char ** _;
            int status = database->get_dbname(fname, _);
            if(status!=0) {
                m_status = modb::DB_ERROR;
                std::cerr << "Error is occured in while reaching database. status:" << status <<std::endl; 
            }
            m_databaseName = std::string(*fname);
            m_database = database; // burada memory leak olabikir
        }

        inline DatabaseResource& operator=(std::nullptr_t){
            m_status = modb::DB_ERROR; 
            return *this;
        }
 
        inline Serializer<T>& Serializer_(){
            return m_serializer;
        }


        SetErrorStream(__DB_STD(ostream) *error_stream) {
            m_database->set_error_stream(&std::cerr);
        }
    
        void Open(DBTYPE type) {
            if(m_databaseName.empty()) {
                m_status = modb::DB_ERROR;
                modb::DatabaseResource<T>::m_SafeModLog("database is empty");
            }
            else {

                try{
                    m_database->open(NULL, m_databaseName.c_str(), NULL, type, DB_CREATE, 0);
                }
                
                catch (std::exception& e)
                {
                    return m_ExceptionForOpening();
                }
            }

            
        }
        inline void WriteKeyValuePair(const std::string& key, const std::string& value) {
            Dbt keyDb = m_ConvertDbt(key);
            Dbt valueDb = m_ConvertDbt(value);
            m_database->put(NULL, &keyDb, &valueDb, 0);
        }
        
        // BulkLoad();
        // BulkWrite();
        inline T& FindById(const std::string&)FindById(const std::string& key) {
            Dbc* cursorp;
            T readRecord;
            m_database->cursor(NULL, &cursorp, 0);

            Dbt keyDb = m_ConvertDbt(key);

            Dbt retVal;
            int ret = cursorp->get(&keyDb, &retVal, DB_SET);

            if (ret) {
                
                readRecord.status = false;
                std::stringstream ss;
                ss << "No records found for '" << key << "'" << std::endl;
                modb::DatabaseResource<T>::m_SafeModLog(ss);
                return readRecord;
            }

            std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};
            
            std::istringstream iss{newObject};
            boost::archive::binary_iarchive ia{iss};


            ia >> readRecord;


            return m_serializer.Deserialize(newObject);
        }

        // UpdateByOid();
        // DeleteByOid();

         ~DatabaseResource() = default;

        private:
            inline void m_ExceptionForOpening() {
                try{
                    throw;
                }
                catch (DbException& e)
                {

                    std::stringstream ss;
                    ss << "Error opening database: " <<  m_databaseName << std::endl;
                    modb::DatabaseResource<T>::m_SafeModLog(ss.str());
                }
                catch (std::exception& e)
                {
                    std::stringstream ss;
                    ss << "Error while opening database without due to berkelydb library File:" << m_databaseName << std::endl;
                    modb::DatabaseResource<T>::m_SafeModLog(ss.str());
                }
            }

            inline void m_SafeModLog(const std::string& logMessage) {
                if (m_isSafe == false) {
                    std::cerr << logMessage << std::endl;  
                }
                else {
                    throw std::runtime_error(logMessage);
                } 
            }

            inline void m_InstantiateSerializer() 
            {
                m_serializer = Serializer<T> {};
            }

            Dbt& m_ConvertDbt(const std::string& value) {
                Dbt valueDb(const_cast<char*>(value.c_str()), static_cast<uint32_t>(value.length() + 1));
                return valueDb;
            }


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