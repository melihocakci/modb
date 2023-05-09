#include <modb/IndexService.h>
#include <exception>
#include <mutex>

#include <spatialindex/SpatialIndex.h>
#include <modb/IndexVisitor.h>

#include <string>

#include <iostream>

using namespace SpatialIndex;

modb::IndexService::IndexService(const std::string& name, modb::DatabaseResource& databaseResource) :
    m_name(name), m_databaseResource{ databaseResource } {
    // burada index oluşturma olacak.

    std::string storageName = name;
    IStorageManager* diskfile = StorageManager::createNewDiskStorageManager(storageName, 4096);

    StorageManager::IBuffer* file = StorageManager::createNewRandomEvictionsBuffer(*diskfile, 10, false);
    // applies a main memory random buffer on top of the persistent storage manager
    // (LRU buffer, etc can be created the same way).

    // Create a new, empty, RTree with dimensionality 2, minimum load 70%, using "file" as
    // the StorageManager and the RSTAR splitting policy.
    id_type indexIdentifier;
    // there is a optimization on this values where very large data comes into play. TODO: reindex 
    ISpatialIndex* tree = RTree::createNewRTree(*file, 0.7, 32, 32, 2, SpatialIndex::RTree::RV_RSTAR, indexIdentifier);

    m_tree = tree;
}

bool modb::IndexService::evaluateObject(modb::Object& object) {
    std::unique_ptr<double[]> plow = object.mbrRegion().pointLow().toDoubleArray();
    std::unique_ptr<double[]> phigh = object.mbrRegion().pointHigh().toDoubleArray();
    SpatialIndex::Region region = SpatialIndex::Region(plow.get(), phigh.get(), 2);

    modb::MyVisitor vis;
    m_tree->intersectsWithQuery(region, vis);
    auto mbr = vis.m_indexes.collection.find(vis.encodeOid2Id(object.id()));
    if (mbr != vis.m_indexes.collection.end())
    {
        return true;
    }

    SpatialIndex::Region m_previousRegion;

    m_previousRegion = region;

    std::ostringstream os;
    SpatialIndex::id_type id = vis.encodeOid2Id(object.id());
    m_tree->deleteData(region, id);
    std::cout << "data is deleted " << id << std::endl;
    std::cout << "converted id " << id << std::endl;

    std::unique_ptr<std::string> point = vis.decodeId2Oid(id);

    std::cout << "my value is " << *point << std::endl;

    m_tree->insertData(0, 0, region, id);

    return false;
}

modb::List<modb::Point> modb::IndexService::intervalQuery(const modb::Point& start, const modb::Point& end)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // false dismissal extraction needed.
    throw std::runtime_error("Not implemented Yet");
}

// it can be extended.i think it is hard to try. We can consult our prof. 
modb::List<modb::Point> modb::IndexService::knnQuery(const modb::Point& point)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // here there also false dismissals when you take closest mbr point and further point .

    throw std::runtime_error("Not implemented Yet");
}

