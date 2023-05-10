#ifndef DATABASERESOURCE_H
#define DATABASERESOURCE_H

#include <modb/Object.h>
#include <modb/IndexService.h>

#include <nlohmann/json.hpp>
#include <db_cxx.h>

#include <iostream>
#include <memory>

using nlohmann::json;
using modb::Object;

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

    class DatabaseResource
    {
    public:
        DatabaseResource(const std::string& dbName, DBTYPE type);
        DatabaseResource(DatabaseResource& other) = default;

        int putObject(const Object& object);
        int updateObject(const Object& object);
        int getObject(const std::string& id, Object& retObject);

        ~DatabaseResource() = default;

    private:
        std::string serialize(const Object& object);
        void deserialize(const std::string& data, Object& object);

        void safeModLog(const std::string&);

        Db m_database; // bdb source, there will be generic class for all DB later
        modb::IndexService m_index;
        std::string m_name;

        RESOURCE_STATUS m_status = modb::DB_NONE;
        // there will be used in tracking object state and 
        // Manager use to this as garbage collector
        bool m_isSafe;
    };
}

#endif
