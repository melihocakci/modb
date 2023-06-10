#include <modb/Object.h>
#include <modb/DatabaseManager.h>
#include <modb/Common.h>
#include <modb/Timer.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <sstream>
#include <functional>

using nlohmann::json;
const std::hash<std::string> hasher;

modb::DatabaseManager::DatabaseManager(const std::string& dbName, DBTYPE dbType, uint32_t flags, double mbrSize) :
    m_index{ dbName },
    m_name{ dbName },
    m_flags{ flags },
    m_mbrSize{ mbrSize },
    m_stats{}
{
    m_env = new DbEnv{ (uint32_t)0 };
    m_env->set_error_stream(&std::cerr);
    m_env->set_cachesize(0, 100 * 1024 * 1024, 0);
    // m_env->set_lg_bsize(10 * 1024 * 1024);
    // m_env->set_lg_max(10 * 1024 * 1024);
    m_env->set_flags(DB_TXN_NOSYNC | DB_NOLOCKING, 1);
    // m_env->set_flags(DB_AUTO_COMMIT, 1);
    m_env->open(".", DB_CREATE | DB_INIT_MPOOL, 0);

    m_database = new Db{ m_env, 0 };
    m_database->set_error_stream(&std::cerr);
    m_database->set_pagesize(4096);
    // m_database->set_flags(DB_REVSPLITOFF);
    // m_database->set_flags(DB_TXN_WRITE_NOSYNC);
    m_database->open(NULL, (m_name + ".db").c_str(), NULL, dbType, m_flags, 0);
}

modb::DatabaseManager::~DatabaseManager() {
    m_database->close(0);
    delete m_database;

    m_env->close(0);
    delete m_env;
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

    int ret;

    {
        modb::Timer timer{&m_stats.dbWriteTime};

        ret = m_database->put(NULL, &key, &value, 0);
    }

    return ret;
}

void modb::DatabaseManager::insertIndex(const int64_t id, const modb::Region& region) {
    modb::Timer timer{&m_stats.idxWriteTime};

    m_index.insertIndex(id, region);
}

void modb::DatabaseManager::deleteIndex(const int64_t id, const modb::Region& region) {
    modb::Timer timer{&m_stats.idxWriteTime};

    m_index.deleteIndex(id, region);
}

int modb::DatabaseManager::getObject(const std::size_t hashedId, Object& retObject) {

    Dbc* cursorp;
    m_database->cursor(NULL, &cursorp, 0);

    Dbt key{ const_cast<std::size_t*>(&hashedId), static_cast<uint32_t>(sizeof(hashedId)) };

    Dbt data;
    int ret;
    {
        modb::Timer timer{&m_stats.dbReadTime};
        ret = cursorp->get(&key, &data, DB_SET);
    }

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

    int ret = getObject(object.id(), oldObject);

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

        insertIndex(hasher(object.id()), newObject.mbrRegion());
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
            deleteIndex(hasher(object.id()), oldObject.mbrRegion());
            insertIndex(hasher(object.id()), newObject.mbrRegion());

            m_stats.idxUpdates++;
        }

        m_stats.dbUpdates++;
    }


    return 0;
}

std::tuple<std::vector<modb::Object>, std::vector<modb::Object>> modb::DatabaseManager::intersectionQuery(const modb::Region& queryRegion) {
    std::vector<SpatialIndex::id_type> indexResults;

    {
        modb::Timer timer{&m_stats.queryTime};

        indexResults = m_index.intersectionQuery(queryRegion);
    }

    std::vector<modb::Object> truePositives{};
    std::vector<modb::Object> falsePositives{};

    {
        modb::Timer timer{&m_stats.filterTime};

        modb::Object object;
        for (SpatialIndex::id_type id : indexResults) {
            getObject(id, object);

            if (pointWithinRegion(object.baseLocation(), queryRegion)) {
                truePositives.push_back(object);
            }
            else {
                falsePositives.push_back(object);
            }
        }
    }

    // for statistics
    m_stats.queries++;
    m_stats.allPositives += indexResults.size();
    m_stats.falsePositives += indexResults.size() - truePositives.size();

    return { truePositives, falsePositives };
}

void modb::DatabaseManager::forEach(std::function<void(const modb::Object& object)> callback) {
    Dbc* cursor;
    m_database->cursor(nullptr, &cursor, 0);

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
    auto stats = std::make_unique<modb::Stats>(m_stats);

    // get bdb statistics
    m_database->stat(nullptr, &stats->dbStats, DB_READ_COMMITTED);

    // get spatialindex statistics
    m_index.getStatistics(&stats->idxStats);

    return stats;
}
