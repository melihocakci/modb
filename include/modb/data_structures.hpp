#pragma once

#include <array>
#include <cstdint>

namespace modb {
    template <int N>
    struct point {
        double arr[N];

        template <class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar& arr;
        }
    };

    template <int N>
    struct rectangle {
        point<N> min;
        point<N> max;

        template <class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar& min;
            ar& max;
        }
    };

    template <typename T, int N>
    struct object {
        int64_t id;
        point<N> location;
        rectangle<N> region;
        T data;

        template <class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar& id;
            ar& location;
            ar& region;
            ar& data;
        }
    };
}
