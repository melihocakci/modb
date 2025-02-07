#include <modb/Region.h>
#include <utility>

using modb::Region;
using modb::Point;

Region::Region() :
    m_pointLow{},
    m_pointHigh{} {}

Region::Region(const Region& other) :
    m_pointLow{ other.m_pointLow },
    m_pointHigh{ other.m_pointHigh } {}

Region::Region(Region&& other) :
    m_pointLow{ std::move(other.m_pointLow) },
    m_pointHigh{ std::move(other.m_pointHigh) } {}

Region::Region(const Point& pointLow, const Point& poingHigh) :
    m_pointLow{ pointLow },
    m_pointHigh{ poingHigh } {}

Region& Region::operator=(const Region& other) {
    m_pointLow = other.m_pointLow;
    m_pointHigh = other.m_pointHigh;

    return *this;
}

Point& Region::pointLow() { return m_pointLow; }
const Point& Region::pointLow() const { return m_pointLow; }

Point& Region::pointHigh() { return m_pointHigh; }
const Point& Region::pointHigh() const { return m_pointHigh; }
