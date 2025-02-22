#pragma once

#include <modb/Region.h>

namespace modb {
    bool pointWithinRegion(const Point& point, const Region& region);
}
