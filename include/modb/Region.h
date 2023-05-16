#ifndef REGION_H
#define REGION_H

#include <modb/Point.h>

#include <nlohmann/json.hpp>

namespace modb {
    class Region {
    public:
        Region();

        Region(const Region& other);

        Region(Region&& other);

        Region(Point pointLow, Point pointHigh);

        Region(const nlohmann::json& mbrRegion);

        ~Region() = default;

        Region& operator=(const Region& other);

        template <class Archieve>
        void serialize(Archieve& ar, unsigned int) {
            ar& m_pointLow;
            ar& m_pointHigh;
        }

        Point& pointLow();
        const Point& pointLow() const;

        Point& pointHigh();
        const Point& pointHigh() const;

    private:
        Point m_pointLow;
        Point m_pointHigh;
    };

}

#endif
