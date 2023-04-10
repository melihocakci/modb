#include <modb/Plane.h>

using Json = nlohmann::json;
using modb::Rectangle;
using modb::Point;

namespace modb {
    Plane::Plane():
        m_oid{},
        m_baseLocation{},
        m_mbrRectangle{} {}

    Plane::Plane(Plane& other):
        m_oid{ other.m_oid },
        m_baseLocation{ other.m_baseLocation },
        m_mbrRectangle{ other.m_mbrRectangle } {}

    Plane::Plane(std::string oid, Point baseLocation, Rectangle mbrRectangle):
        m_oid{ oid },
        m_baseLocation{ baseLocation },
        m_mbrRectangle{ mbrRectangle } {}

    Plane& Plane::operator=(Plane& other) {
        m_oid = other.m_oid;
        m_baseLocation = other.m_baseLocation;
        m_mbrRectangle = other.m_mbrRectangle;

        return *this;
    }

    Plane::Plane(Json json):
        m_oid{ json["oid"] },
        m_baseLocation{ json["baseLocation"] },
        m_mbrRectangle{ json["mbrRectangle"] } {}

    bool Plane::Plane::SetJson(Json json) { // tehre will be second parameter about schema in the future
        std::cout << "json logic can be here" << std::endl;
        return true;
    }

    std::string Plane::oid() { return m_oid; }

    Point Plane::baseLocation() { return m_baseLocation; }

    Rectangle Plane::mbrRectangle() { return m_mbrRectangle; }
}
