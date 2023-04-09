#include <modb/Point.h>

#include <utility>

namespace modb {
    Point::Point():
        longitude{},
        latitude{} {}

    Point::Point(Point& other):
        longitude{ other.longitude },
        latitude{ other.latitude } {}

    Point::Point(Point&& other):
        longitude{ std::exchange(other.longitude, 0) },
        latitude{ std::exchange(other.latitude, 0) } {}

    Point::Point(float longitude, float latitude):
        longitude{ longitude },
        latitude{ latitude } {}
}