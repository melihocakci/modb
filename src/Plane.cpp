#include <modb/Plane.h>

#include <utility>

namespace modb {
    Plane::Plane():
        oid{},
        baseLocation{},
        mbrLocation{},
        mbrWidth{} {}

    Plane::Plane(Plane& other):
        oid{ other.oid },
        baseLocation{ other.baseLocation },
        mbrLocation{ other.mbrLocation },
        mbrWidth{ other.mbrWidth } {}

    Plane::Plane(Plane&& other):
        oid{ std::move(other.oid) },
        baseLocation{ std::move(other.baseLocation) },
        mbrLocation{ std::move(other.mbrLocation) },
        mbrWidth{ std::exchange(other.mbrWidth, 0) } {}

    Plane::Plane(std::string oid, Point baseLocation, Point mbrLocation, float mbrWidth):
        oid{ oid },
        baseLocation{ baseLocation },
        mbrLocation{ mbrLocation },
        mbrWidth{ mbrWidth } {}
}