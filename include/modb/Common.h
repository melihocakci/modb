#ifndef COMMON_H
#define COMMON_H

#include <modb/Region.h>

bool pointWithinRegion(const modb::Point& point, const modb::Region& region);

#endif
