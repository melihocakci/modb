#pragma once

#include <modb/data_structures.hpp>
#include <modb/db_container.hpp>
#include <modb/idx_container.hpp>
#include <spatialindex/SpatialIndex.h>
#include <db_cxx.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <mutex>
#include <string>
#include <utility>

namespace modb
{
    template <typename T, int N> requires (N > 0)
        class database
    {
    public:
        database(const std::string& name) noexcept :
            db{ name },
            idx{ name, N }
        {
        }

        std::pair<int, modb::object<T, N>> get_object(const int64_t id) const noexcept {
            Dbc* cursor;
            db->cursor(NULL, &cursor, 0);

            Dbt key{ id, sizeof(id) };
            Dbt value;
            int ret = cursor->get(&key, &value, DB_SET);

            if (ret) {
                return { ret, {} };
            }

            std::string_view data_string{ value.get_data(), value.get_size() };
            return { 0, deserialize(data_string) };
        }

        int put_object(const int64_t id, const modb::point<N>& location, const T& data) const noexcept {
            std::pair<int, modb::object<T, N>> result = get_object(id);

            bool keep_region = result.first == 0 && intersects(location, result.second.region);

            modb::object<T, N> obj{ id, location, keep_region ? result.second.region : generate_region(location), data };

            int ret = put_object(obj);
            if (ret) {
                return ret;
            }

            if (!keep_region) {
                insert_index(obj.id, obj.region);
            }

            return 0;
        }

        // std::tuple<std::vector<modb::Object>, std::vector<modb::Object>> intersection_query(const modb::rectangle<N>& query_region) {
        //     std::vector<SpatialIndex::id_type> indexResults;


        //     indexResults = intersection_query(query_region);

        //     std::vector<modb::Object> truePositives{};
        //     std::vector<modb::Object> falsePositives{};

        //     {
        //         modb::Timer timer{ &m_stats.filterTime };

        //         modb::Object object;
        //         for (SpatialIndex::id_type id : indexResults) {
        //             getObject(id, object);

        //             if (pointWithinRegion(object.baseLocation(), queryRegion)) {
        //                 truePositives.push_back(object);
        //             }
        //             else {
        //                 falsePositives.push_back(object);
        //             }
        //         }
        //     }

        //     // for statistics
        //     m_stats.queries++;
        //     m_stats.allPositives += indexResults.size();
        //     m_stats.falsePositives += indexResults.size() - truePositives.size();

        //     return { truePositives, falsePositives };
        // }

        // void forEach(std::function<void(const modb::Object& object)> callback) {
        //     Dbc* cursor;
        //     m_database->cursor(nullptr, &cursor, 0);

        //     Dbt key, data;
        //     while (cursor->get(&key, &data, DB_NEXT) == 0) {
        //         modb::Object object;

        //         deserialize(std::string(static_cast<char*>(data.get_data()), data.get_size()), object);

        //         callback(object);
        //     }

        //     cursor->close();
        // }

        // void queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy) {
        //     m_index.queryStrategy(queryStrategy);
        // }

        // std::unique_ptr<modb::Stats> getStats() {
        //     auto stats = std::make_unique<modb::Stats>(m_stats);

        //     // get bdb statistics
        //     m_database->stat(nullptr, &stats->dbStats, DB_READ_COMMITTED);

        //     // get spatialindex statistics
        //     m_index.getStatistics(&stats->idxStats);

        //     return stats;
        // }

    private:
        bool intersects(const modb::point<N>& location, const modb::rectangle<N>& region) const noexcept {
            for (int i = 0; i < N; i++) {
                if (location.coordinates[i] < region.min.coordinates[i] || location.coordinates[i] > region.max.coordinates[i]) {
                    return false;
                }
            }

            return true;
        }

        modb::rectangle<N> generate_region(const modb::point<N>& location) const noexcept {
            constexpr double half_size = 0.15;
            modb::rectangle<N> region;

            for (int i = 0; i < N; i++) {
                region.min.coordinates[i] = location.coordinates[i] - half_size;
                region.max.coordinates[i] = location.coordinates[i] + half_size;
            }

            return region;
        }

        std::string serialize(const modb::object<T, N>& obj) const noexcept {
            std::ostringstream output_stream{};
            boost::archive::binary_oarchive output_archive{ output_stream };

            output_archive << obj;

            return output_stream.str();
        }

        modb::object<T, N> deserialize(std::string_view data) const noexcept {
            std::istringstream input_stream{ data };
            boost::archive::binary_iarchive input_archive{ input_stream };

            modb::object<T, N> obj;
            input_archive >> obj;

            return obj;
        }

        int put_object(const modb::object<T, N>& obj) const noexcept {
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

        SpatialIndex::Region to_spatial_region(const modb::rectangle<N>& region) const noexcept {
            return SpatialIndex::Region{ region.min, region.max, N };
        }

        void delete_index(const int64_t id, const modb::rectangle<N>& region) const noexcept {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->deleteData(spatial_region, id);
        }

        void insert_index(const int64_t id, const modb::rectangle<N>& region) const noexcept {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->insertData(0, nullptr, spatial_region, id);
        }

        std::vector<int64_t> intersection_query(const modb::rectangle<N>& query_region) const noexcept {
            SpatialIndex::Region spatial_region = to_spatial_region(query_region);
            index_visitor vis;

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->intersectsWithQuery(spatial_region, vis);

            return vis.query_result;
        }

        // todo: add knn query. It is not implemented in the original code.


        modb::db_container db;
        modb::idx_container idx;

        std::mutex idx_lock;
    };
}
