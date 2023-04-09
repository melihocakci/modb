#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>

#include <string>

namespace modb {
    class Plane {
    public:
        Plane();
        

        Plane(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, float mbrWidth);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_oid;
            ar& m_baseLocation;
            ar& m_mbrLocation;
            ar& m_mbrWidth;
        }

        std::string getOid();

        modb::Point getBaseLocation();

        modb::Point getMbrLocation();

        float getMbrWidth();

    private:
        std::string m_oid;

        modb::Point m_baseLocation;

        modb::Point m_mbrLocation;

        float m_mbrWidth;
    };
}

#endif