#ifndef POINT_H
#define POINT_H

namespace modb {
    class Point {
    public:
        float longitude;
        float latitude;

        Point();

        Point(Point& other);

        Point(Point&& other);

        Point(float longitude, float latitude);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& longitude;
            ar& latitude;
        }
    };
}

#endif