#include <modb/Object.h>

#include <iostream>

using modb::Object;
using modb::Region;
using modb::Point;

Object::Object() :
    m_oid{},
    m_baseLocation{},
    m_mbrRegion{} {}

Object::Object(const Object& other) :
    m_oid{ other.m_oid },
    m_baseLocation{ other.m_baseLocation },
    m_mbrRegion{ other.m_mbrRegion } {}

Object::Object(Object&& other) :
    m_oid{ std::move(other.m_oid) },
    m_baseLocation{ std::move(other.m_baseLocation) },
    m_mbrRegion{ std::move(other.m_mbrRegion) } {}

Object::Object(std::string id, Point baseLocation, Region mbrRegion) :
    m_oid{ id },
    m_baseLocation{ baseLocation },
    m_mbrRegion{ mbrRegion } {}

Object::Object(std::string id, Point baseLocation) :
    m_oid{ id },
    m_baseLocation{ baseLocation },
    m_mbrRegion{} {}

Object& Object::operator=(const Object& other) {
    m_oid = other.m_oid;
    m_baseLocation = other.m_baseLocation;
    m_mbrRegion = other.m_mbrRegion;

    return *this;
}

std::string& Object::id() { return m_oid; }
const std::string& Object::id() const { return m_oid; }

Point& Object::baseLocation() { return m_baseLocation; }
const Point& Object::baseLocation() const { return m_baseLocation; }

Region& Object::mbrRegion() { return m_mbrRegion; }
const Region& Object::mbrRegion() const { return m_mbrRegion; }
