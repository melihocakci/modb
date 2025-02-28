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

            int ret = put_object_db(obj);
            if (ret) {
                return ret;
            }

            if (!keep_region) {
                insert_index(obj.id, obj.region);
            }

            return 0;
        }

        std::vector<modb::object<T, N>> intersection_query(const modb::rectangle<N>& query_region) const noexcept {
            std::vector<int64_t> index_results = intersection_query_idx(query_region);

            std::vector<modb::object<T, N>> true_positives;

            for (int64_t id : index_results) {
                modb::object obj = get_object(id);

                if (intersects(obj.location, query_region)) {
                    true_positives.push_back(std::move(obj));
                }
            }

            return true_positives;
        }

        void for_each(std::function<void(const modb::object<T, N>& obj)> callback) const noexcept {
            Dbc* cursor;
            db->cursor(nullptr, &cursor, 0);

            Dbt key, data;
            while (cursor->get(&key, &data, DB_NEXT) == 0) {
                callback(deserialize(data.get_data(), data.get_size()));
            }

            cursor->close();
        }

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

        int put_object_db(const modb::object<T, N>& obj) const noexcept {
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

        std::vector<int64_t> intersection_query_idx(const modb::rectangle<N>& query_region) const noexcept {
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
