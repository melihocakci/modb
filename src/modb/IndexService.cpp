#include <modb/IndexService.h>
#include <modb/IndexVisitor.h>

#include <spatialindex/SpatialIndex.h>

#include <exception>
#include <mutex>
#include <string>
#include <iostream>
#include <filesystem>

using namespace SpatialIndex;

modb::IndexService::IndexService(const std::string& name) :
    m_name{ name }
{
    std::string baseName = name;

    std::filesystem::path idxfile{ name + ".idx" };
    std::filesystem::path datfile{ name + ".dat" };

    if (std::filesystem::exists(idxfile) && std::filesystem::exists(datfile)) {
        // load existing index
        m_diskfile = StorageManager::loadDiskStorageManager(baseName);
        // this will try to locate and open an already existing storage manager.

        m_file = StorageManager::createNewRandomEvictionsBuffer(*m_diskfile, 10, false);
        // applies a main memory random buffer on top of the persistent storage manager
        // (LRU buffer, etc can be created the same way).

        // If we need to open an existing tree stored in the storage manager, we only
        // have to specify the index identifier as follows
        m_rtree = RTree::loadRTree(*m_file, 1);
    }
    else {
        // create a new index
        m_diskfile = StorageManager::createNewDiskStorageManager(baseName, 4096);

        m_file = StorageManager::createNewRandomEvictionsBuffer(*m_diskfile, 10, false);
        // applies a main memory random buffer on top of the persistent storage manager
        // (LRU buffer, etc can be created the same way).

        // Create a new, empty, RTree with dimensionality 2, minimum load 70%, using "file" as
        // the StorageManager and the RSTAR splitting policy.
        id_type indexIdentifier;
        // there is a optimization on this values where very large data comes into play. TODO: reindex 
        m_rtree = RTree::createNewRTree(*m_file, 0.7, 32, 32, 2, SpatialIndex::RTree::RV_RSTAR, indexIdentifier);
    }
}

modb::IndexService::~IndexService() {
    // necessary to save the index file properly upon exit
    std::lock_guard<std::mutex> lock{m_mutex};
    delete m_rtree;
    delete m_file;
    delete m_diskfile;
}

inline SpatialIndex::Region toSpatialRegion(const modb::Region& modbRegion) {
    double plow[]{ modbRegion.pointLow().longitude(), modbRegion.pointLow().latitude() };
    double phigh[]{ modbRegion.pointHigh().longitude(), modbRegion.pointHigh().latitude() };

    return SpatialIndex::Region{plow, phigh, 2};
}

void modb::IndexService::deleteIndex(const int64_t id, const modb::Region& region) {
    SpatialIndex::Region spatialRegion = toSpatialRegion(region);

    std::lock_guard<std::mutex> guard{m_mutex};
    m_rtree->deleteData(spatialRegion, id);
}

void modb::IndexService::insertIndex(const int64_t id, const modb::Region& region) {
    SpatialIndex::Region spatialRegion = toSpatialRegion(region);

    std::lock_guard<std::mutex> guard{m_mutex};
    m_rtree->insertData(0, 0, spatialRegion, id);
}

std::vector<SpatialIndex::id_type> modb::IndexService::intersectionQuery(const modb::Region& queryRegion)
{
    SpatialIndex::Region region = toSpatialRegion(queryRegion);

    modb::IndexVisitor vis;

    std::lock_guard<std::mutex> guard{m_mutex};
    m_rtree->intersectsWithQuery(region, vis);

    return vis.m_queryResult;
}

// // it can be extended.i think it is hard to try. We can consult our prof. 
// modb::List<modb::Point> modb::IndexService::knnQuery(const modb::Point& point)
// {
//     std::lock_guard<std::mutex> lock(m_mutex);

//     // here there also false dismissals when you take closest mbr point and further point .

//     throw std::runtime_error("Not implemented Yet");
// }

void modb::IndexService::queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy) {
    m_rtree->queryStrategy(queryStrategy);
}

void modb::IndexService::getStatistics(SpatialIndex::IStatistics** stats) {
    m_rtree->getStatistics(stats);
}
