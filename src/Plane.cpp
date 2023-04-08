#include <modb/Plane.h>

modb::Plane::Plane(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, double mbrWidth):
    m_oid{ oid }, m_mbrWidth{ mbrWidth } {
    m_baseLocation = baseLocation;
    m_mbrLocation = mbrLocation;
}

std::string modb::Plane::getOid() { return m_oid; }