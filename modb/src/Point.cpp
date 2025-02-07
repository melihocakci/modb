#include <modb/Point.h>

#include <utility>
#include <memory>

using modb::Point;

Point::Point() :
    m_longitude{},
    m_latitude{} {}

Point::Point(const Point& other) :
    m_longitude{ other.m_longitude },
    m_latitude{ other.m_latitude } {}

Point::Point(Point&& other) :
    m_longitude{ std::exchange(other.m_longitude, 0) },
    m_latitude{ std::exchange(other.m_latitude, 0) } {}

Point::Point(double longitude, double latitude) :
    m_longitude{ longitude },
    m_latitude{ latitude } {}

Point& Point::operator=(const Point& other) {
    m_longitude = other.m_longitude;
    m_latitude = other.m_latitude;

    return *this;
}

double& Point::longitude() { return m_longitude; }
const double& Point::longitude() const { return m_longitude; }

double& Point::latitude() { return m_latitude; }
const double& Point::latitude() const { return m_latitude; }
