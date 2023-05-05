#include <modb/IndexService.h>
#include <exception>
#include <mutex>

#include <spatialindex/SpatialIndex.h>
#include <modb/MyVisitor.h>

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
    double* plow = object.mbrRegion().pointLow().toDoubleArray();
    double* phigh = object.mbrRegion().pointLow().toDoubleArray();
    SpatialIndex::Region region = SpatialIndex::Region(plow, phigh, 2);

    modb::MyVisitor vis;
    // m_tree->intersectsWithQuery(region, vis);
    auto mbr = vis.m_indexes.collection.find(vis.encodeOid2Id(object.id()));
    if(mbr != vis.m_indexes.collection.end())
    {
        return false;
    }
     
    SpatialIndex::Region m_previousRegion;

    m_previousRegion = region;

    std::ostringstream os;
    
    std::string data = os.str();
    SpatialIndex::id_type id;


    id = vis.encodeOid2Id(object.id());
    std::cout << "converted id " << id << std::endl;
    
    std::unique_ptr<std::string> point = vis.decodeId2Oid(id);

    std::cout << "my value is "<< *point << std::endl; 

    m_tree->insertData(0, 0, region, id);


     

    // m_tree->intersectsWithQuery(m_previousRegion, vis);


    // // burada index oluştuktan sonra 
    // std::lock_guard<std::mutex> lock(m_mutex);
    // object.baseLocation();
    // // double plow[2], phigh[2];
    // Point center = object.baseLocation();

    // modb::Object prevObj;

    // MyVisitor vis;
    // double plow[2] = {object.baseLocation().latitude()-0.01, object.baseLocation().longitude()-0.01};
    // double phigh[2] = {object.baseLocation().latitude()+0.01, object.baseLocation().longitude()+0.01};

    // SpatialIndex::Region region = SpatialIndex::Region(plow, phigh, 2);
    // m_tree->intersectsWithQuery(region, vis);

    

    // int isEmpty = m_databaseResource.getObject(object.id(), prevObj);
    // if(isEmpty) {
    //     // 0.01 lik mbr oluştur sonraki iterasyonda büyüyecek.

    // } else {

    // }


    // SpatialIndex::Region dataRegion = SpatialIndex::Region();
    // std::ostringstream os;
    // os << dataRegion;
    // std::string data = os.str();


    // // m_tree->insertData((uint32_t)(data.size() + 1), reinterpret_cast<const uint8_t*>(data.c_str()), r, object.id());


    return false;
}

modb::List<modb::Point> modb::IndexService::intervalQuery(const modb::Point& start, const modb::Point& end)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    throw std::runtime_error("Not implemented yet");

}

modb::List<modb::Point> modb::IndexService::knnQuery(const modb::Point& point) 
{
    std::lock_guard<std::mutex> lock(m_mutex);
    throw std::runtime_error("Not implemented Yet");
}

