#ifndef INDEXSERVICE_H
#define INDEXSERVICE_H

#include <modb/Object.h>

#include <modb/Point.h>
#include <mutex>  

#include <spatialindex/SpatialIndex.h>

using namespace SpatialIndex;

namespace modb
{
    class IndexService
    {
    public:
        IndexService(const std::string& name);

        ~IndexService();

        // when service evaluating object and writting to tree. Someone from api can query
        // bool evaluateObject(Object& object); // insertion logic in r-tree occurs here

        void deleteIndex(const int64_t id, const Region& region);
        void insertIndex(const int64_t id, const Region& region);

        std::vector<SpatialIndex::id_type> intersectionQuery(const Region& queryRegion);

        // modb::List<Point> knnQuery(const Point& point); // query closest n point to that point or up to n point
        // modb::List<Point> intersectionQuery(const Point& start, const Point& end); // query points in interval of two point


        // debug purpose add after
        // void setQueryStrategy(SpatialIndex::IQueryStrategy queryStrategy);


    private:
        IStorageManager* m_diskfile;
        StorageManager::IBuffer* m_file;
        ISpatialIndex* m_rtree;

        std::string m_name;
        std::mutex m_mutex; // libspatial index is not thread-safe.
    };

}

#endif
