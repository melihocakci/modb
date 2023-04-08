#ifndef POINT_H
#define POINT_H

namespace modb {
    class Point {
    public:
        double longitude;
        double latitude;

        ~Point() = default;

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& longitude;
            ar& latitude;
        }
    };
}

#endif