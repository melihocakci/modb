#ifndef INDEXSERVICE_H
#define INDEXSERVICE_H

#include <modb/Object.h>
#include <modb/Collections.h>

#include <modb/DatabaseResource.h>
#include <modb/Point.h>
#include <mutex>  

#include <spatialindex/SpatialIndex.h>

using namespace SpatialIndex;

namespace modb
{
    class IndexService
    {
    public:
        IndexService(const std::string& name, modb::DatabaseResource& databaseResource);
        
        // when service evaluating object and writting to tree. Someone from api can query
        bool evaluateObject(modb::Object& object); // insertion logic in r-tree occurs here

        modb::List<Point> knnQuery(const Point& point); // query closest n point to that point or up to n point
        modb::List<Point> intervalQuery(const Point& start, const Point& end); // query points in interval of two point



    private:
        const std::string& m_name;
        modb::DatabaseResource& m_databaseResource;
        std::mutex m_mutex; // libspatial index is not thread-safe.
        ISpatialIndex* m_tree;
    };

}


#endif