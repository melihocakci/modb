#include <modb/Plane.h>

namespace modb {
    Plane::Plane():  
            m_oid(""),
            m_baseLocation (),
            m_mbrLocation (),
            m_mbrWidth(0.0)
    {}
    Plane::Plane(std::string oid, Point baseLocation, Point mbrLocation, float mbrWidth):
        m_oid{ oid }, m_mbrWidth{ mbrWidth }, m_baseLocation{ baseLocation }, m_mbrLocation{ mbrLocation } {}

    std::string Plane::Oid() { return m_oid; }

    Point Plane::BaseLocation() { return m_baseLocation; }

    Point Plane::MbrLocation() { return m_mbrLocation; }

    float Plane::MbrWidth() { return m_mbrWidth; }
}
