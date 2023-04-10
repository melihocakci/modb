#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>
#include <modb/DatabaseResource.h>
#include <string>

namespace modb {
    class Plane : public modb::DataObject {
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

        std::string Oid();

        modb::Point BaseLocation();

        modb::Point MbrLocation();

        float MbrWidth();

    private:
        std::string m_oid;

        modb::Point m_baseLocation;

        modb::Point m_mbrLocation;

        float m_mbrWidth;
    };
}

#endif