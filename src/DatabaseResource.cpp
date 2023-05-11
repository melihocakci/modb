#include <modb/Object.h>
#include <modb/DatabaseResource.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <sstream>
#include <functional>

using nlohmann::json;
const std::hash<std::string> hasher;

bool pointWithinRegion(const modb::Point& point, const modb::Region& region);

modb::DatabaseResource::DatabaseResource(const std::string& dbName, DBTYPE dbType) :
    m_database{ NULL, 0 },
    m_index{ dbName },
    m_name{ dbName }
{
    m_database.set_error_stream(&std::cerr);
    m_database.open(NULL, (m_name + ".db").c_str(), NULL, dbType, DB_CREATE, 0);
}

std::string modb::DatabaseResource::serialize(const modb::Object& object) {
    std::ostringstream outputStream{};
    boost::archive::binary_oarchive outputArchive(outputStream);

    outputArchive << object;

    return outputStream.str();
}

void modb::DatabaseResource::deserialize(const std::string& data, modb::Object& object) {
    std::istringstream inputStream{data};
    boost::archive::binary_iarchive inputArchive{inputStream};

    inputArchive >> object;
}

int modb::DatabaseResource::putObject(const Object& object) {
    std::size_t hashedId = hasher(object.id());
    std::string objectData = serialize(object);

    Dbt key{ &hashedId, static_cast<uint32_t>(sizeof(hashedId)) };
    Dbt value{ const_cast<char*>(objectData.data()), static_cast<uint32_t>(objectData.length()) };

    int ret = m_database.put(NULL, &key, &value, 0);

    return ret;
}

int modb::DatabaseResource::getObject(const std::size_t hashedId, Object& retObject) {
    Dbc* cursorp;
    m_database.cursor(NULL, &cursorp, 0);

    Dbt key{ const_cast<std::size_t*>(&hashedId), static_cast<uint32_t>(sizeof(hashedId)) };

    Dbt retVal;
    int ret = cursorp->get(&key, &retVal, DB_SET);

    if (ret) {
        // empty result
        return ret;
    }

    std::string objectData{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

    deserialize(objectData, retObject);

    return ret;
}

int modb::DatabaseResource::getObject(const std::string& id, Object& retObject) {
    return getObject(hasher(id), retObject);
}

void modb::DatabaseResource::safeModLog(const std::string& logMessage) {
    if (m_isSafe == false) {
        std::cerr << logMessage << std::endl;
    }
    else {
        throw std::runtime_error(logMessage);
    }
}

int modb::DatabaseResource::updateObject(const Object& object) {
    modb::Object oldObject{};

    int ret = getObject(object.id(), oldObject);

    if (ret) // object not found 
    {
        modb::Object newObject{ object };

        double longitude = object.baseLocation().longitude();
        double latitude = object.baseLocation().latitude();

        newObject.mbrRegion() = {
            {longitude - 0.3, latitude - 0.3},
            {longitude + 0.3, latitude + 0.3},
        };

        putObject(newObject);

        m_index.insertIndex(hasher(object.id()), newObject.mbrRegion());
    }
    else // object found
    {
        modb::Object newObject{ object };

        if (pointWithinRegion(newObject.baseLocation(), oldObject.mbrRegion())) {
            newObject.mbrRegion() = oldObject.mbrRegion();
            putObject(newObject);
        }
        else {
            double longitude = object.baseLocation().longitude();
            double latitude = object.baseLocation().latitude();

            newObject.mbrRegion() = {
                {longitude - 0.3, latitude - 0.3},
                {longitude + 0.3, latitude + 0.3},
            };

            putObject(newObject);

            // update the index
            m_index.deleteIndex(hasher(object.id()), oldObject.mbrRegion());
            m_index.insertIndex(hasher(object.id()), newObject.mbrRegion());
        }
    }

    return 0;
}

std::vector<std::string> modb::DatabaseResource::intersectionQuery(const modb::Region& queryRegion) {
    std::vector<SpatialIndex::id_type> indexResults = m_index.intersectionQuery(queryRegion);
    std::vector<std::string> queryResults{};

    modb::Object object;
    for (SpatialIndex::id_type id : indexResults) {
        getObject(id, object);

        if (pointWithinRegion(object.baseLocation(), queryRegion)) {
            queryResults.push_back(object.id());
        }
    }

    return queryResults;
}


bool pointWithinRegion(const modb::Point& point, const modb::Region& region) {
    if (point.longitude() < region.pointHigh().longitude()
        && point.longitude() > region.pointLow().longitude()
        && point.latitude() < region.pointHigh().latitude()
        && point.latitude() > region.pointLow().latitude())
    {
        return true;
    }
    else
    {
        return false;
    }
}