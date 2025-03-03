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

        std::pair<int, modb::object<T, N>> get_object(int64_t id) const noexcept {
            Dbc* cursor;
            db->cursor(NULL, &cursor, 0);

            Dbt key{ &id, sizeof(id) };
            Dbt value;
            int ret = cursor->get(&key, &value, DB_SET);

            if (ret) {
                return { ret, {} };
            }

            const std::string data_string{ reinterpret_cast<char*>(value.get_data()), value.get_size() };
            return { 0, deserialize(data_string) };
        }

        int put_object(const int64_t id, const modb::point<N>& location, const T& data) const noexcept {
            const auto [get_ret, old_obj] = get_object(id);

            bool update_region = get_ret != 0 || (get_ret == 0 && !intersects(location, old_obj.region));

            modb::object<T, N> new_obj{ id, location, update_region ? generate_region(location) : old_obj.region, data };

            int put_ret = put_object_db(new_obj);
            if (put_ret) {
                return put_ret;
            }

            if (update_region) {
                if (get_ret == 0) {
                    delete_index(id, old_obj.region);
                }

                insert_index(id, new_obj.region);
            }

            return 0;
        }

        std::vector<modb::object<T, N>> intersection_query(const modb::rectangle<N>& query_region) const noexcept {
            if (!is_valid(query_region)) {
                return {};
            }

            std::vector<int64_t> index_results = intersection_query_idx(query_region);

            std::vector<modb::object<T, N>> true_positives;

            for (int64_t id : index_results) {
                auto [ret, obj] = get_object(id);

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
                callback(deserialize({ data.get_data(), data.get_size() }));
            }

            cursor->close();
        }

    private:
        bool is_valid(const modb::rectangle<N>& region) const noexcept {
            for (int i = 0; i < N; i++) {
                if (region.min.arr[i] > region.max.arr[i]) {
                    return false;
                }
            }

            return true;
        }

        bool intersects(const modb::point<N>& location, const modb::rectangle<N>& region) const noexcept {
            for (int i = 0; i < N; i++) {
                if (location.arr[i] < region.min.arr[i] || location.arr[i] > region.max.arr[i]) {
                    return false;
                }
            }

            return true;
        }

        modb::rectangle<N> generate_region(const modb::point<N>& location) const noexcept {
            constexpr double half_size = 0.15;
            modb::rectangle<N> region;

            for (int i = 0; i < N; i++) {
                region.min.arr[i] = location.arr[i] - half_size;
                region.max.arr[i] = location.arr[i] + half_size;
            }

            return region;
        }

        std::string serialize(const modb::object<T, N>& obj) const noexcept {
            std::ostringstream output_stream{};
            boost::archive::binary_oarchive output_archive{ output_stream };

            output_archive << obj;

            return output_stream.str();
        }

        modb::object<T, N> deserialize(const std::string& data) const noexcept {
            std::istringstream input_stream{ data };
            boost::archive::binary_iarchive input_archive{ input_stream };

            modb::object<T, N> obj;
            input_archive >> obj;

            return obj;
        }

        int put_object_db(modb::object<T, N> obj) const noexcept {
            std::string object_data = serialize(obj);

            Dbt key{ &obj.id, sizeof(obj.id) };
            Dbt value{ object_data.data(), static_cast<u_int32_t>(object_data.length()) };

            return db->put(NULL, &key, &value, 0);
        }

        SpatialIndex::Region to_spatial_region(const modb::rectangle<N>& region) const noexcept {
            return SpatialIndex::Region{ region.min.arr, region.max.arr, N };
        }

        bool delete_index(const int64_t id, const modb::rectangle<N>& region) const noexcept {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            return idx->deleteData(spatial_region, id);
        }

        void insert_index(const int64_t id, const modb::rectangle<N>& region) const noexcept {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ idx_lock };
            idx->insertData(0, nullptr, spatial_region, id);
        }

        class index_visitor : public SpatialIndex::IVisitor {
        public:
            std::vector<int64_t> query_result;

            void visitNode(const SpatialIndex::INode& d) override {}

            void visitData(const SpatialIndex::IData& d) override {
                query_result.push_back(d.getIdentifier());
            }

            void visitData(std::vector<const SpatialIndex::IData*>& v) {}
        };

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

        mutable std::mutex idx_lock;
    };
}
