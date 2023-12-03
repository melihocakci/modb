#include <modb/Common.h>

bool modb::pointWithinRegion(const modb::Point& point, const modb::Region& region) {
    if (point.longitude() < region.pointHigh().longitude()
        && point.longitude() > region.pointLow().longitude()
        && point.latitude() < region.pointHigh().latitude()
        && point.latitude() > region.pointLow().latitude())
    {
        return true;
    }
    else
    {
        return false;
    }
}
