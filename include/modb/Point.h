#ifndef POINT_H
#define POINT_H

namespace modb {
    class Point {
    public:
        double longitude;
        double latitude;

        ~Point() = default;
    };
}

#endif