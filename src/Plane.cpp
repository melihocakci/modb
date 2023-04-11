#include <modb/Plane.h>

using Json = nlohmann::json;

namespace modb {
    Plane::Plane():
        m_oid{},
        m_baseLocation{},
        m_mbrRegion{} {}

    Plane::Plane(Plane& other):
        m_oid{ other.m_oid },
        m_baseLocation{ other.m_baseLocation },
        m_mbrRegion{ other.m_mbrRegion } {}

    Plane::Plane(std::string oid, Point baseLocation, Rectangle mbrRegion):
        m_oid{ oid },
        m_baseLocation{ baseLocation },
        m_mbrRegion{ mbrRegion } {}

    Plane& Plane::operator=(Plane& other) {
        m_oid = other.m_oid;
        m_baseLocation = other.m_baseLocation;
        m_mbrRegion = other.m_mbrRegion;

        return *this;
    }

    Plane::Plane(Json json):
        m_oid{ json["oid"] },
        m_baseLocation{ json["baseLocation"] },
        m_mbrRegion{ json["mbrRegion"] } {}

    bool Plane::Plane::SetJson(Json json) { // tehre will be second parameter about schema in the future
        std::cout << "json logic can be here" << std::endl;
        return true;
    }

    std::string Plane::oid() { return m_oid; }

    Point Plane::baseLocation() { return m_baseLocation; }

    Rectangle Plane::mbrRegion() { return m_mbrRegion; }
}
