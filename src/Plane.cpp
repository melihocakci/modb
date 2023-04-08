#include <modb/Plane.h>

namespace modb {
    Plane::Plane(std::string oid, Point baseLocation, Point mbrLocation, float mbrWidth):
        m_oid{ oid }, m_mbrWidth{ mbrWidth }, m_baseLocation{ baseLocation }, m_mbrLocation{ mbrLocation } {}

    std::string Plane::getOid() { return m_oid; }

    Point Plane::getBaseLocation() { return m_baseLocation; }

    Point Plane::getMbrLocation() { return m_mbrLocation; }

    float Plane::getMbrWidth() { return m_mbrWidth; }
}
