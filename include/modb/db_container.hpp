#pragma once

#include <db_cxx.h>
#include <string>

namespace modb
{
    class db_container
    {
    public:
        db_container(const std::string& name) noexcept
        {
            db_env = new DbEnv{ 0u };
            db_env->set_error_stream(&std::cerr);
            db_env->set_cache_max(0, 400 * 1024 * 1024);
            db_env->set_cachesize(0, 200 * 1024 * 1024, 0);
            db_env->set_lg_bsize(10 * 1024 * 1024);
            db_env->set_lg_max(10 * 1024 * 1024);
            db_env->set_flags(DB_TXN_NOSYNC | DB_NOLOCKING, 1);
            // db_env->set_flags(DB_AUTO_COMMIT, 1);
            db_env->open(".", DB_CREATE | DB_INIT_MPOOL | DB_PRIVATE, 0);

            db = new Db{ db_env, 0u };
            db->set_error_stream(&std::cerr);
            db->set_pagesize(4096);
            // db->set_flags(DB_REVSPLITOFF);
            // db->set_flags(DB_TXN_WRITE_NOSYNC);
            db->open(NULL, (name + ".db").c_str(), NULL, DB_BTREE, DB_CREATE, 0);
        }

        ~db_container() noexcept {
            if (db) {
                db->close(0);
                delete db;
            }

            if (db_env) {
                db_env->close(0);
                delete db_env;
            }
        }

        Db* operator->() const noexcept {
            return db;
        }

    private:
        DbEnv* db_env;
        Db* db;
    };
}
