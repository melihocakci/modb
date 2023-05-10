#ifndef POINT_H
#define POINT_H

#include <nlohmann/json.hpp>

namespace modb {
    class Point {
    public:
        Point();

        Point(const Point& other);

        Point(Point&& other);

        Point(float longitude, float latitude);

        Point(const nlohmann::json& baseLocation);

        ~Point() = default;

        Point& operator=(const Point& other);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_longitude;
            ar& m_latitude;
        }

        float& longitude();
        const float& longitude() const;

        float& latitude();
        const float& latitude() const;

    private:
        float m_longitude;
        float m_latitude;
    };
}

#endif
