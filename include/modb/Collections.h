#ifndef COLLECTION_H
#define COLLECTION_H

#include <vector>
#include <set>

namespace modb 
{
    template<class T>
    struct List
    {
        std::vector<T> collection;
    };

    template <class T>
    struct Set
    {
        std::set<T> collection;
    };

}

#endif
