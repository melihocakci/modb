#pragma once

#include <modb/data_structures.hpp>
#include <spatialindex/SpatialIndex.h>
#include <mutex>
#include <string_view>
#include <filesystem>

namespace modb
{
    namespace fs = std::filesystem;

    template <int N>
    class index_manager
    {
    public:
        index_manager(std::string index_name):
        {
            if (fs::exists(index_name + ".idx") && fs::exists(index_name + ".dat")) {
                // load existing index
                disk_file = SpatialIndex::StorageManager::loadDiskStorageManager(index_name);
                // this will try to locate and open an already existing storage manager.

                buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*disk_file, 10, false);
                // applies a main memory random buffer on top of the persistent storage manager
                // (LRU buffer, etc can be created the same way).

                // If we need to open an existing tree stored in the storage manager, we only
                // have to specify the index identifier as follows
                rtree = SpatialIndex::RTree::loadRTree(*buffer, 1);
            }
            else {
                // create a new index
                disk_file = SpatialIndex::StorageManager::createNewDiskStorageManager(baseName, 4096);

                buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*disk_file, 10, false);
                // applies a main memory random buffer on top of the persistent storage manager
                // (LRU buffer, etc can be created the same way).

                // Create a new, empty, RTree with dimensionality 2, minimum load 70%, using "file" as
                // the StorageManager and the RSTAR splitting policy.
                SpatialIndex::id_type indexIdentifier;
                // there is a optimization on this values where very large data comes into play. TODO: reindex 
                rtree = SpatialIndex::RTree::createNewRTree(*buffer, 0.7, 32, 32, N, SpatialIndex::RTree::RV_RSTAR, indexIdentifier);
            }
        }

        ~index_manager() {
            // necessary to save the index file properly upon exit
            std::lock_guard<std::mutex> lock{ mut };
            delete rtree;
            delete buffer;
            delete disk_file;
        }

        SpatialIndex::Region to_spatial_region(const modb::rectangle<N>& region) {
            return SpatialIndex::Region{ region.min, region.max, N };
        }

        void delete_index(const int64_t id, const modb::rectangle<N>& region) {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ mut };
            rtree->deleteData(spatial_region, id);
        }

        void insert_index(const int64_t id, const modb::rectangle<N>& region) {
            SpatialIndex::Region spatial_region = to_spatial_region(region);

            std::lock_guard<std::mutex> guard{ mut };
            rtree->insertData(0, 0, spatial_region, id);
        }

        std::vector<SpatialIndex::id_type> intersection_query(const modb::rectangle<N>& query_region)
        {
            SpatialIndex::Region spatial_region = to_spatial_region(query_region);
            modb::IndexVisitor vis;

            std::lock_guard<std::mutex> guard{ mut };
            rtree->intersectsWithQuery(spatial_region, vis);

            return vis.m_queryResult;
        }

        // // it can be extended.i think it is hard to try. We can consult our prof. 
        // modb::List<modb::point> knnQuery(const modb::point& point)
        // {
        //     std::lock_guard<std::mutex> lock(mut);

        //     // here there also false dismissals when you take closest mbr point and further point .

        //     throw std::runtime_error("Not implemented Yet");
        // }

        void query_strategy(SpatialIndex::IQueryStrategy& queryStrategy) {
            rtree->queryStrategy(queryStrategy);
        }

        void get_statistics(SpatialIndex::IStatistics** stats) {
            rtree->getStatistics(stats);
        }

    private:
        SpatialIndex::IStorageManager* disk_file;
        SpatialIndex::StorageManager::IBuffer* buffer;
        SpatialIndex::ISpatialIndex* rtree;

        std::mutex mut; // libspatial index is not thread-safe.
    };
}
