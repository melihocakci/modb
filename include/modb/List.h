#ifndef LIST_H
#define LIST_H

#include <vector>

namespace modb 
{
    template<class T>
    struct List
    {
        std::vector<T> array;
    };

}

#endif