#ifndef INDEXSERVICE_H
#define INDEXSERVICE_H

#include <modb/Object.h>

#include <modb/Point.h>
#include <mutex>  

#include <spatialindex/SpatialIndex.h>

namespace modb
{
    class IndexService
    {
    public:
        IndexService(const std::string& name);

        ~IndexService();

        // when service evaluating object and writting to tree. Someone from api can query
        // bool evaluateObject(Object& object); // insertion logic in r-tree occurs here

        void deleteIndex(const int64_t id, const modb::Region& region);
        void insertIndex(const int64_t id, const modb::Region& region);

        std::vector<SpatialIndex::id_type> intersectionQuery(const modb::Region& queryRegion);

        void queryStrategy(SpatialIndex::IQueryStrategy& queryStrategy);

        void getStatistics(SpatialIndex::IStatistics**);

        // modb::List<Point> knnQuery(const Point& point); // query closest n point to that point or up to n point
        // modb::List<Point> intersectionQuery(const Point& start, const Point& end); // query points in interval of two point


        // debug purpose add after
        // void setQueryStrategy(SpatialIndex::IQueryStrategy queryStrategy);


    private:
        SpatialIndex::IStorageManager* m_diskfile;
        SpatialIndex::StorageManager::IBuffer* m_file;
        SpatialIndex::ISpatialIndex* m_rtree;

        std::string m_name;
        std::mutex m_mutex; // libspatial index is not thread-safe.
    };

}

#endif
