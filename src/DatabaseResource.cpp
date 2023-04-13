#include <modb/Object.h>
#include <modb/DatabaseResource.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <sstream>

using nlohmann::json;
using modb::Object;

modb::DatabaseResource::DatabaseResource(const std::string& dbName, DBTYPE dbType):
    m_database{ NULL, 0 },
    m_databaseName{ dbName }
{
    m_database.set_error_stream(&std::cerr);
    m_database.open(NULL, m_databaseName.c_str(), NULL, dbType, DB_CREATE, 0);
}

std::string modb::DatabaseResource::serialize(const Object& object) {
    std::ostringstream outputStream{};
    boost::archive::binary_oarchive outputArchive(outputStream);

    outputArchive << object;

    return outputStream.str();
}

void modb::DatabaseResource::deserialize(const std::string& data, Object& object) {
    std::istringstream inputStream{data};
    boost::archive::binary_iarchive inputArchive{inputStream};

    inputArchive >> object;
}

int modb::DatabaseResource::putObject(const Object& object) {
    const std::string& objectId = object.id();

    std::string objectData = serialize(object);

    Dbt key{ const_cast<char*>(objectId.data()), static_cast<uint32_t>(objectId.length()) };
    Dbt value{ const_cast<char*>(objectData.data()), static_cast<uint32_t>(objectData.length()) };

    int ret = m_database.put(NULL, &key, &value, 0);

    return ret;
}

int modb::DatabaseResource::getObject(const std::string& id, Object& retObject) {
    Dbc* cursorp;
    m_database.cursor(NULL, &cursorp, 0);

    Dbt key{ const_cast<char*>(id.data()), static_cast<uint32_t>(id.length()) };

    Dbt retVal;
    int ret = cursorp->get(&key, &retVal, DB_SET);

    if (ret) {
        // empty result
        return ret;
    }

    std::string objectData{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

    std::istringstream inputStream{objectData};
    boost::archive::binary_iarchive inputArchive{inputStream};

    inputArchive >> retObject;

    return ret;
}

void modb::DatabaseResource::safeModLog(const std::string& logMessage) {
    if (m_isSafe == false) {
        std::cerr << logMessage << std::endl;
    }
    else {
        throw std::runtime_error(logMessage);
    }
}
