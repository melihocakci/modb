#include <modb/Region.h>

using nlohmann::json;
using modb::Region;
using modb::Point;

Region::Region():
    m_pointLow{},
    m_pointHigh{} {}

Region::Region(Region& other):
    m_pointLow{ other.m_pointLow },
    m_pointHigh{ other.m_pointHigh } {}

Region::Region(Region&& other):
    m_pointLow{ std::move(other.m_pointLow) },
    m_pointHigh{ std::move(other.m_pointHigh) } {}

Region::Region(Point pointLow, Point poingHigh):
    m_pointLow{ pointLow },
    m_pointHigh{ poingHigh } {}

Region::Region(const json& mbrRegion):
    m_pointLow{ mbrRegion["pointLow"] },
    m_pointHigh{ mbrRegion["pointHigh"] } {}

Region& Region::operator=(Region& other) {
    m_pointLow = other.m_pointLow;
    m_pointHigh = other.m_pointHigh;

    return *this;
}

Point Region::pointLow() { return m_pointLow; }


Point Region::pointHigh() { return m_pointHigh; }
