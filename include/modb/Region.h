#ifndef REGION_H
#define REGION_H

#include <modb/Point.h>

#include <nlohmann/json.hpp>

namespace modb {

    class Region {
    public:
        Region();

        Region(Region& other);

        Region(Region&& other);

        Region(Point pointLow, Point pointHigh);

        Region(nlohmann::json mbrRegion);

        ~Region() = default;

        Region& operator=(Region& other);

        template <class Archieve>
        inline void serialize(Archieve& ar, unsigned int) {
            ar& m_pointLow;
            ar& m_pointHigh;
        }

        Point pointLow();
        Point pointHigh();

    private:
        Point m_pointLow;
        Point m_pointHigh;
    };

}

#endif