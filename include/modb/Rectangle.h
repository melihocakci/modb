#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <modb/Point.h>

#include <nlohmann/json.hpp>

namespace modb {

    class Rectangle {
    public:
        Rectangle();

        Rectangle(Rectangle& other);

        Rectangle(Point pointLow, Point pointHigh);

        Rectangle(nlohmann::json mbrRectangle);

        ~Rectangle() = default;

        Rectangle& operator=(Rectangle& other);

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