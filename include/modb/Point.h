#ifndef POINT_H
#define POINT_H

#include <nlohmann/json.hpp>

namespace modb {
    class Point {
    public:
        Point();

        Point(Point& other);

        Point(Point&& other);

        Point(float longitude, float latitude);

        Point(const nlohmann::json& baseLocation);

        ~Point() = default;

        Point& operator=(Point& other);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_longitude;
            ar& m_latitude;
        }

        float longitude();
        float latitude();

        std::unique_ptr<double[]> toDoubleArray();
    private:
        float m_longitude;
        float m_latitude;
    };
}

#endif
