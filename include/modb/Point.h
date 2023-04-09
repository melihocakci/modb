#ifndef POINT_H
#define POINT_H

namespace modb {
    class Point {
    public:
        float longitude;
        float latitude;

        Point() = default;

        Point(Point& other) = default;

        ~Point() = default;

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& longitude;
            ar& latitude;
        }
    };
}

#endif