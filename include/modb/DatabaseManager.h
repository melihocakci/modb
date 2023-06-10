#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <modb/Object.h>
#include <modb/IndexManager.h>

#include <nlohmann/json.hpp>
#include <db_cxx.h>

#include <iostream>
#include <functional>
#include <memory>
#include <tuple>
#include <chrono>
#include <atomic>

namespace modb {
    typedef enum {
        DB_NONE = 1,
        DB_OPENED = 2, // first state
        DB_OVERWRITTEN = 3, // if db there and overwritten it
        DB_FORKED = 4, // if database is copied it is state is forked 
        DB_ERROR = 5, // if any opening error occurs
        DB_UNKNOWN = 6 // others 
    } RESOURCE_STATUS;

    typedef enum {
        WRITE_DEFAULT = 0,
        WRITE_APPEND = 2,
        WRITE_NODUPDATA = 19,
        WRITE_NOOVERWRITE = 20,
        WRITE_WRITEMULTIPLE = 2048,
        WRITE_MULTIPLE_KEY = 16384,
        WRITE_OVERWRITE_DUP = 21,

    } RECORD_WRITE_OPTION;

    struct Stats {
        int dbUpdates;
        int idxUpdates;
        int queries;
        int allPositives;
        int falsePositives;
        std::unique_ptr<DB_BTREE_STAT> dbStats;
        std::unique_ptr<SpatialIndex::IStatistics> idxStats;
        int64_t dbPutTime;
        int64_t dbGetTime;
        int64_t idxPutTime;
        int64_t queryTime;
        int64_t filterTime;
    };

    class DatabaseManager
    {
    public:
        DatabaseManager(const std::string& dbName, DBTYPE type = DB_BTREE, uint32_t flags = DB_CREATE, double mbrSize = 0.3);
        DatabaseManager(DatabaseManager& other) = default;

        int putObject(const Object& object);

        int getObject(const std::size_t id, Object& retObject);
        int getObject(const std::string& id, Object& retObject);

        std::vector<Object> intersectionQuery(const Region& queryRegion);

        void forEach(std::function<void(const Object& object)> callback);

        void queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy);

        std::unique_ptr<modb::Stats> getStats();

        ~DatabaseManager() = default;

    private:
        int putObjectDB(const Object& object);

        std::string serialize(const Object& object);
        void deserialize(const std::string& data, Object& object);

        void safeModLog(const std::string&);

        Db m_database; // bdb source, there will be generic class for all DB later
        modb::IndexManager m_index;
        std::string m_name;
        uint32_t m_flags;

        RESOURCE_STATUS m_status = modb::DB_NONE;
        // there will be used in tracking object state and 
        // Manager use to this as garbage collector
        bool m_isSafe;

        double m_mbrSize;

        // statistic members
        std::atomic<int> m_dbUpdates;
        std::atomic<int> m_idxUpdates;
    
        std::atomic<int> m_queries;
        std::atomic<int> m_allPositives;
        std::atomic<int> m_falsePositives;

        std::atomic<int64_t> m_dbPutTime;
        std::atomic<int64_t> m_dbGetTime;
        std::atomic<int64_t> m_idxPutTime;
        std::atomic<int64_t> m_queryTime;
        std::atomic<int64_t> m_filterTime;
    };
}


#endif