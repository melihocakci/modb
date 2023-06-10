#include <modb/Object.h>
#include <modb/DatabaseManager.h>
#include <modb/Common.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <sstream>
#include <functional>

using nlohmann::json;
const std::hash<std::string> hasher;

modb::DatabaseManager::DatabaseManager(const std::string& dbName, DBTYPE dbType, uint32_t flags, double mbrSize) :
    m_database{ NULL, 0 },
    m_index{ dbName },
    m_name{ dbName },
    m_flags{ flags },
    m_mbrSize{ mbrSize },
    m_dbUpdates{ 0 },
    m_idxUpdates{ 0 },
    m_queries{ 0 },
    m_allPositives{ 0 },
    m_falsePositives{ 0 },
    m_dbPutTime{ 0 },
    m_dbGetTime{ 0 },
    m_idxPutTime{ 0 },
    m_queryTime{ 0 },
    m_filterTime{ 0 }
{
    m_database.set_error_stream(&std::cerr);
    m_database.open(NULL, (m_name + ".db").c_str(), NULL, dbType, m_flags, 0);
}

std::string modb::DatabaseManager::serialize(const modb::Object& object) {
    std::ostringstream outputStream{};
    boost::archive::binary_oarchive outputArchive{outputStream};

    outputArchive << object;

    return outputStream.str();
}

void modb::DatabaseManager::deserialize(const std::string& data, modb::Object& object) {
    std::istringstream inputStream{data};
    boost::archive::binary_iarchive inputArchive{inputStream};

    inputArchive >> object;
}

int modb::DatabaseManager::putObjectDB(const Object& object) {
    std::size_t hashedId = hasher(object.id());
    std::string objectData = serialize(object);

    Dbt key{ &hashedId, static_cast<uint32_t>(sizeof(hashedId)) };
    Dbt value{ const_cast<char*>(objectData.data()), static_cast<uint32_t>(objectData.length()) };

    int ret = m_database.put(NULL, &key, &value, 0);

    return ret;
}

int modb::DatabaseManager::getObject(const std::size_t hashedId, Object& retObject) {
    Dbc* cursorp;
    m_database.cursor(NULL, &cursorp, 0);

    Dbt key{ const_cast<std::size_t*>(&hashedId), static_cast<uint32_t>(sizeof(hashedId)) };

    Dbt data;
    int ret = cursorp->get(&key, &data, DB_SET);

    if (ret) {
        // empty result
        return ret;
    }

    std::string objectData{reinterpret_cast<char*>(data.get_data()), data.get_size()};

    deserialize(objectData, retObject);

    return ret;
}

int modb::DatabaseManager::getObject(const std::string& id, Object& retObject) {
    return getObject(hasher(id), retObject);
}

void modb::DatabaseManager::safeModLog(const std::string& logMessage) {
    if (m_isSafe == false) {
        std::cerr << logMessage << std::endl;
    }
    else {
        throw std::runtime_error(logMessage);
    }
}

int modb::DatabaseManager::putObject(const Object& object) {
    modb::Object oldObject{};

    // auto start = std::chrono::system_clock::now();
    int ret = getObject(object.id(), oldObject);
    // auto end = std::chrono::system_clock::now();

    // auto diff = end - start;
    // m_dbGetTime += diff.count();

    if (ret) // object not found 
    {
        modb::Object newObject{ object };

        double longitude = object.baseLocation().longitude();
        double latitude = object.baseLocation().latitude();

        newObject.mbrRegion() = {
            {longitude - m_mbrSize / 2, latitude - m_mbrSize / 2},
            {longitude + m_mbrSize / 2, latitude + m_mbrSize / 2},
        };

        ret = putObjectDB(newObject);

        if (ret) {
            return ret;
        }

        m_index.insertIndex(hasher(object.id()), newObject.mbrRegion());
    }
    else // object found
    {
        modb::Object newObject{ object };

        if (pointWithinRegion(newObject.baseLocation(), oldObject.mbrRegion())) {
            newObject.mbrRegion() = oldObject.mbrRegion();
            putObjectDB(newObject);
        }
        else {
            double longitude = object.baseLocation().longitude();
            double latitude = object.baseLocation().latitude();

            // to be changed to a heuristic function
            newObject.mbrRegion() = {
                {longitude - m_mbrSize / 2, latitude - m_mbrSize / 2},
                {longitude + m_mbrSize / 2, latitude + m_mbrSize / 2},
            };

            ret = putObjectDB(newObject);

            if (ret) {
                return ret;
            }

            // update the index
            m_index.deleteIndex(hasher(object.id()), oldObject.mbrRegion());
            m_index.insertIndex(hasher(object.id()), newObject.mbrRegion());

            m_idxUpdates++;
        }

        m_dbUpdates++;
    }


    return 0;
}

std::vector<modb::Object> modb::DatabaseManager::intersectionQuery(const modb::Region& queryRegion) {
    std::vector<SpatialIndex::id_type> indexResults = m_index.intersectionQuery(queryRegion);
    std::vector<modb::Object> filteredResults{};

    modb::Object object;
    for (SpatialIndex::id_type id : indexResults) {
        getObject(id, object);

        if (pointWithinRegion(object.baseLocation(), queryRegion)) {
            filteredResults.push_back(object);
        }
    }

    // for statistics
    m_queries++;
    m_allPositives += indexResults.size();
    m_falsePositives += indexResults.size() - filteredResults.size();

    return filteredResults;
}

void modb::DatabaseManager::forEach(std::function<void(const modb::Object& object)> callback) {
    Dbc* cursor;
    m_database.cursor(nullptr, &cursor, 0);

    Dbt key, data;
    while (cursor->get(&key, &data, DB_NEXT) == 0) {
        modb::Object object;

        deserialize(std::string(static_cast<char*>(data.get_data()), data.get_size()), object);

        callback(object);
    }

    cursor->close();
}


void modb::DatabaseManager::queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy) {
    m_index.queryStrategy(queryStrategy);
}

std::unique_ptr<modb::Stats> modb::DatabaseManager::getStats() {
    auto stats = std::make_unique<modb::Stats>();

    stats->dbUpdates = m_dbUpdates;
    stats->idxUpdates = m_idxUpdates;
    stats->queries = m_queries;
    stats->allPositives = m_allPositives;
    stats->falsePositives = m_falsePositives;

    // get bdb statistics
    DB_BTREE_STAT* dbStats;
    m_database.stat(nullptr, &dbStats, DB_READ_COMMITTED);
    stats->dbStats = std::unique_ptr<DB_BTREE_STAT>{ dbStats };

    // get spatialindex statistics
    SpatialIndex::IStatistics* idxStats;
    m_index.getStatistics(&idxStats);
    stats->idxStats = std::unique_ptr<SpatialIndex::IStatistics>{ idxStats };

    return stats;
}