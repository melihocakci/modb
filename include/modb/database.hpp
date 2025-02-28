#pragma once

#include <modb/data_structures.hpp>
#include <spatialindex/SpatialIndex.h>
#include <db_cxx.h>
#include <mutex>
#include <filesystem>
#include <memory>
#include <utility>

namespace modb
{
    namespace fs = std::filesystem;

    template <typename T, int N> requires (N > 0)
        class database
    {
    public:
        database(std::string_view db_name) :
            db_name{ db_name }
        {
            initiate_db();
            initiate_index();
        }

        void initiate_db() {
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
            db->open(NULL, (db_name + ".db").c_str(), NULL, DB_BTREE, DB_CREATE, 0);
        }

        void initiate_index() {
            if (fs::exists(db_name + ".idx") && fs::exists(db_name + ".dat")) {
                idx_file = SpatialIndex::StorageManager::loadDiskStorageManager(db_name);
                idx_buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*idx_file, 10, false);
                idx = SpatialIndex::RTree::loadRTree(*idx_buffer, 1);
            }
            else {
                idx_file = SpatialIndex::StorageManager::createNewDiskStorageManager(db_name, 4096);
                idx_buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*idx_file, 10, false);
                SpatialIndex::id_type index_identifier;
                idx = SpatialIndex::RTree::createNewRTree(*idx_buffer, 0.7, 32, 32, N, SpatialIndex::RTree::RV_RSTAR, index_identifier);
            }
        }

        ~database() {
            if (db) {
                db->close(0);
                delete db;
            }

            if (db_env) {
                db_env->close(0);
                delete db_env;
            }

            delete idx;
            delete idx_buffer;
            delete idx_file;
        }

        std::pair<int, modb::object<T, N>> get_object(const int64_t id) {
            Dbc* cursor;
            db->cursor(NULL, &cursor, 0);

            Dbt key{ id, sizeof(id) };
            Dbt value;
            int ret = cursor->get(&key, &value, DB_SET);

            if (ret) {
                return { ret, {} };
            }

            std::string_view data_string{ reinterpret_cast<char*>(value.get_data()), value.get_size() };
            return { 0, deserialize(data_string) };
        }

        int put_object(const int64_t id, const modb::point<N>& location, const T& data) {
            auto result = get_object(object.id);

            bool keep_region = result.first == 0 && intersects(location, result.second.region);

            modb::object<T, N> obj{ id, location, keep_region ? result.second.region : generate_region(location), data };

            int ret = insert_object(obj);
            if (ret) {
                return ret;
            }

            if (!keep_region) {
                insert_index(obj.id, obj.region);
            }

            return 0;
        }

        std::tuple<std::vector<modb::Object>, std::vector<modb::Object>> intersection_query(const modb::rectangle<N>& query_region) {
            std::vector<SpatialIndex::id_type> indexResults;

   
            indexResults = intersection_query(query_region);

            std::vector<modb::Object> truePositives{};
            std::vector<modb::Object> falsePositives{};

            {
                modb::Timer timer{ &m_stats.filterTime };

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

        void forEach(std::function<void(const modb::Object& object)> callback) {
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

        void queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy) {
            m_index.queryStrategy(queryStrategy);
        }

        std::unique_ptr<modb::Stats> getStats() {
            auto stats = std::make_unique<modb::Stats>(m_stats);

            // get bdb statistics
            m_database->stat(nullptr, &stats->dbStats, DB_READ_COMMITTED);

            // get spatialindex statistics
            m_index.getStatistics(&stats->idxStats);

            return stats;
        }

    private:
        bool intersects(const modb::point<N>& location, const modb::rectangle<N>& region) {
            for (int i = 0; i < N; i++) {
                if (location.coordinates[i] < region.min.coordinates[i] || location.coordinates[i] > region.max.coordinates[i]) {
                    return false;
                }
            }

            return true;
        }

        modb::rectangle<N> generate_region(const modb::point<N>& location) {
            constexpr double half_size = 0.15;
            modb::rectangle<N> region;

            for (int i = 0; i < N; i++) {
                region.min.coordinates[i] = location.coordinates[i] - half_size;
                region.max.coordinates[i] = location.coordinates[i] + half_size;
            }

            return region;
        }

        std::string serialize(const modb::object<T, N>& obj) {
            std::ostringstream outputStream{};
            boost::archive::binary_oarchive outputArchive{ outputStream };

            outputArchive << obj;

            return outputStream.str();
        }

        modb::object<T, N> deserialize(std::string_view data) {
            std::istringstream inputStream{ data };
            boost::archive::binary_iarchive inputArchive{ inputStream };

            modb::object<T, N> obj;
            inputArchive >> obj;

            return obj;
        }

        int insert_object(const modb::object<T, N>& obj) {
            std::string object_data = serialize(obj);

            Dbt key{ &obj.id, sizeof(obj.id) };
            Dbt value{ object_data.data(), object_data.length() };

            return db->put(NULL, &key, &value, 0);
        }

        class index_visitor : public SpatialIndex::IVisitor {
        public:
            std::vector<int64_t> query_result;

            void visitData(const SpatialIndex::IData& d) override {
                query_result.push_back(d.getIdentifier());
            }
        };

        SpatialIndex::Region to_spatial_region(const modb::rectangle<N>& region) {
            return SpatialIndex::Region{ region.min, region.max, N };
        }

        void delete_index(const int64_t id, const modb::rectangle<N>& region) {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->deleteData(spatial_region, id);
        }

        void insert_index(const int64_t id, const modb::rectangle<N>& region) {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->insertData(0, nullptr, spatial_region, id);
        }

        std::vector<int64_t> intersection_query(const modb::rectangle<N>& query_region)
        {
            SpatialIndex::Region spatial_region = to_spatial_region(query_region);
            index_visitor vis;

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->intersectsWithQuery(spatial_region, vis);

            return vis.query_result;
        }

        // todo: add knn query. It is not implemented in the original code.


        std::string db_name;
        std::mutex idx_lock;

        DbEnv* db_env;
        Db* db;

        SpatialIndex::IStorageManager* idx_file;
        SpatialIndex::StorageManager::IBuffer* idx_buffer;
        SpatialIndex::ISpatialIndex* idx;
    };
}
