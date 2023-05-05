#include <modb/Point.h>

#include <utility>

using nlohmann::json;
using modb::Point;

Point::Point():
    m_longitude{},
    m_latitude{} {}

Point::Point(Point& other):
    m_longitude{ other.m_longitude },
    m_latitude{ other.m_latitude } {}

Point::Point(Point&& other):
    m_longitude{ std::exchange(other.m_longitude, 0) },
    m_latitude{ std::exchange(other.m_latitude, 0) } {}

Point::Point(float longitude, float latitude):
    m_longitude{ longitude },
    m_latitude{ latitude } {}

Point& Point::operator=(Point& other) {
    m_longitude = other.m_longitude;
    m_latitude = other.m_latitude;

    return *this;
}

Point::Point(const json& baseLocation):
    m_longitude{ baseLocation["longitude"] },
    m_latitude{ baseLocation["latitude"] } {}

double* Point::toDoubleArray() { 
    double * arr = new double[2];
    arr[0] = m_longitude; 
    arr[1] = m_latitude;
    return arr;
}

float Point::longitude() { return m_longitude; }

float Point::latitude() { return m_latitude; }