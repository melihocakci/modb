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

        Plane() = default;

        Plane(Plane& other) = default;

        ~Plane() = default;

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