#include <modb/Object.h>

using nlohmann::json;

namespace modb {
    Object::Object():
        m_oid{},
        m_baseLocation{},
        m_mbrRegion{} {}

    Object::Object(Object& other):
        m_oid{ other.m_oid },
        m_baseLocation{ other.m_baseLocation },
        m_mbrRegion{ other.m_mbrRegion } {}

    Object::Object(Object&& other):
        m_oid{ std::move(other.m_oid) },
        m_baseLocation{ std::move(other.m_baseLocation) },
        m_mbrRegion{ std::move(other.m_mbrRegion) } {}

    Object::Object(std::string id, Point baseLocation, Region mbrRegion):
        m_oid{ id },
        m_baseLocation{ baseLocation },
        m_mbrRegion{ mbrRegion } {}

    Object& Object::operator=(Object& other) {
        m_oid = other.m_oid;
        m_baseLocation = other.m_baseLocation;
        m_mbrRegion = other.m_mbrRegion;

        return *this;
    }

    Object::Object(json json):
        m_oid{ json["id"] },
        m_baseLocation{ json["baseLocation"] },
        m_mbrRegion{ /*json["mbrRegion"]*/ } {}

    bool Object::Object::SetJson(json json) { // tehre will be second parameter about schema in the future
        std::cout << "json logic can be here" << std::endl;
        return true;
    }

    std::string Object::id() { return m_oid; }

    Point Object::baseLocation() { return m_baseLocation; }

    Region Object::mbrRegion() { return m_mbrRegion; }
}
