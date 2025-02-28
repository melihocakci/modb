#pragma once

#include <spatialindex/SpatialIndex.h>
#include <filesystem>
#include <string>

namespace modb
{
    class idx_container
    {
    public:
        idx_container(std::string name, int dimension) noexcept
        {
            namespace fs = std::filesystem;

            if (fs::exists(name + ".idx") && fs::exists(name + ".dat")) {
                idx_file = SpatialIndex::StorageManager::loadDiskStorageManager(name);
                idx_buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*idx_file, 10, false);
                idx = SpatialIndex::RTree::loadRTree(*idx_buffer, 1);
            }
            else {
                idx_file = SpatialIndex::StorageManager::createNewDiskStorageManager(name, 4096);
                idx_buffer = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*idx_file, 10, false);
                SpatialIndex::id_type index_identifier;
                idx = SpatialIndex::RTree::createNewRTree(*idx_buffer, 0.7, 32, 32, dimension, SpatialIndex::RTree::RV_RSTAR, index_identifier);
            }
        }

        ~idx_container() noexcept {
            delete idx;
            delete idx_buffer;
            delete idx_file;
        }

        SpatialIndex::ISpatialIndex* operator->() const noexcept {
            return idx;
        }

    private:
        SpatialIndex::IStorageManager* idx_file;
        SpatialIndex::StorageManager::IBuffer* idx_buffer;
        SpatialIndex::ISpatialIndex* idx;
    };
}
