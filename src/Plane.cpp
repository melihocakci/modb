#include <modb/Plane.h>

using Json = nlohmann::json;
using modb::Rectangle;
using modb::Point;

namespace modb {
    Plane::Plane():
        m_oid{},
        m_baseLocation{},
        m_mbrRegion{} {}

    Plane::Plane(Plane& other):
        m_oid{ other.m_oid },
        m_baseLocation{ other.m_baseLocation },
        m_mbrRegion{ other.m_mbrRegion } {}

    Plane::Plane(std::string oid, Point baseLocation, Rectangle mbrRectangle):
        m_oid{ oid },
        m_baseLocation{ baseLocation },
        m_mbrRegion{ mbrRectangle } {}

    Plane& Plane::operator=(Plane& other) {
        m_oid = other.m_oid;
        m_baseLocation = other.m_baseLocation;
        m_mbrRegion = other.m_mbrRegion;

        return *this;
    }

    Plane::Plane(Json json):
        m_oid{ json["oid"] },
        m_baseLocation{  json["baseLocation"]["longitude"].get<float>(), json["baseLocation"]["latitude"].get<float>() },
        m_mbrRegion{ {json["mbrRegion"]["startPoint"]["longitude"].get<float>(),
                      json["mbrRegion"]["startPoint"]["latitude"].get<float>() 
                     },
                     {json["mbrRegion"]["endPoint"]["longitude"].get<float>(),
                      json["mbrRegion"]["endPoint"]["latitude"].get<float>()
                     }
                    } {

                        std::cout << "plane json construction completed" << std::endl;
                    }

    bool Plane::Plane::SetJson(Json json) { // tehre will be second parameter about schema in the future
        std::cout << "json logic can be here" << std::endl;
        return true;
    }

    std::string Plane::oid() { return m_oid; }

    Point Plane::baseLocation() { return m_baseLocation; }

    Rectangle Plane::mbrRectangle() { return m_mbrRegion; }
}
