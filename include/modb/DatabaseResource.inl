#ifndef DATABASERESOURCE_INL
#define DATABASERESOURCE_INL

#include <modb/DatabaseResource.h>
#include <sstream>


template<typename T>  std::string& modb::Serializer<T>::Serialize(T data) {
    T plane{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, 1.1 };

    std::ostringstream oss{};
    boost::archive::binary_oarchive oa(oss);


    oa << plane;
    std::string serialized{oss.str()};


    m_serializedData = serialized;
    return m_serializedData;
}

template<typename T> modb::Serializer<T>::~Serializer() {
    m_serializedData.clear();

    // NOTE :: there is a function to delete Generic data implementation in memory

    // delete &m_serializedData;
    // delete &m_deserializedData;
}

template<typename T> T& modb::Serializer<T>::Deserialize(std::string& serializedData) {
    std::istringstream iss{serializedData};
    boost::archive::binary_iarchive ia{iss};

    T readRecord;

    ia >> readRecord;
    
    m_data = readRecord;

    return m_data;
}

template<typename T> modb::Serializer<T>&  modb::DatabaseResource<T>::Serializer_(){
    return m_serializer;
}

template<typename T> void modb::DatabaseResource<T>::m_InstantiateSerializer() 
{
    m_serializer = Serializer<T> {};
}

template<typename T> modb::DatabaseResource<T>::DatabaseResource() {
    m_InstantiateSerializer();
}

template<typename T> modb::DatabaseResource<T>::DatabaseResource(const std::string& dbName, DBTYPE type) {
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

template<typename T> void modb::DatabaseResource<T>::m_ExceptionForOpening() {
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



template<typename T> void modb::DatabaseResource<T>::WriteKeyValuePair(const std::string& key, const std::string& value) {
    Dbt keyDb = m_ConvertDbt(key);
    Dbt valueDb = m_ConvertDbt(value);
    m_database->put(NULL, &keyDb, &valueDb, 0);
}

template <typename T> Dbt& modb::DatabaseResource<T>::m_ConvertDbt(const std::string& value) {
    Dbt valueDb(const_cast<char*>(value.c_str()), static_cast<uint32_t>(value.length() + 1));
    return valueDb;
}

template <typename T> T modb::Serializer<T>::GetData() {
    return m_data;
}

template <typename T> std::string& modb::Serializer<T>::GetSerializedData() {
    return m_serializedData;
}

template<typename T> T& modb::DatabaseResource<T>::FindById(const std::string& key) {
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
        modb::DatabaseResource<T>::m_SafeModLog(ss.str());
        
        readRecord = m_serializer.GetData();
        return readRecord;
    }

    std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};
    
    std::istringstream iss{newObject};
    boost::archive::binary_iarchive ia{iss};


    ia >> readRecord;


    return m_serializer.Deserialize(newObject);
}

template<typename T> modb::DatabaseResource<T>::DatabaseResource(Db* database) : m_database(database) {
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
}

// sinking error flag from outside
template<typename T> modb::DatabaseResource<T>& modb::DatabaseResource<T>::operator=(std::nullptr_t) {
    m_status = modb::DB_ERROR; 
    return *this;
}


template<typename T> void modb::DatabaseResource<T>::SetErrorStream(__DB_STD(ostream) *error_stream) {
    m_database->set_error_stream(&std::cerr);
}

template<typename T> void modb::DatabaseResource<T>::m_SafeModLog(const std::string& logMessage) {
    if (m_isSafe == false) {
        std::cerr << logMessage << std::endl;  
    }
    else {
        throw std::runtime_error(logMessage);
    } 
}

template<typename T> void modb::DatabaseResource<T>::Open(DBTYPE type) {
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

#endif