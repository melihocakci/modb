#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>

#include <string>

namespace modb {
    class Plane {
    public:
        std::string oid;
        Point baseLocation;
        Point mbrLocation;
        float mbrWidth;

        Plane();

        Plane(Plane& other);

        Plane(Plane&& other);

        Plane(std::string oid, Point baseLocation, Point mbrLocation, float mbrWidth);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& oid;
            ar& baseLocation;
            ar& mbrLocation;
            ar& mbrWidth;
        }
    };
}

#endif