#ifndef COMMON_H
#define COMMON_H

#include <modb/Region.h>

namespace modb {
    bool pointWithinRegion(const Point& point, const Region& region);
}

#endif
