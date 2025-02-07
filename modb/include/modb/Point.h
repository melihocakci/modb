#pragma once

namespace modb {
    class Point {
    public:
        Point();

        Point(const Point& other);

        Point(Point&& other);

        Point(double longitude, double latitude);

        ~Point() = default;

        Point& operator=(const Point& other);

        template <class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar& m_longitude;
            ar& m_latitude;
        }

        double& longitude();
        const double& longitude() const;

        double& latitude();
        const double& latitude() const;

    private:
        double m_longitude;
        double m_latitude;
    };
}
