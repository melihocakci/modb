#include <modb/Rectangle.h>

namespace modb {

    Rectangle::Rectangle():
        m_pointLow{},
        m_pointHigh{} {}

    Rectangle::Rectangle(Rectangle& other):
        m_pointLow{ other.pointLow() },
        m_pointHigh{ other.pointHigh() } {}

    Rectangle::Rectangle(Point pointLow, Point poingHigh):
        m_pointLow{ pointLow },
        m_pointHigh{ poingHigh } {}

    Rectangle::Rectangle(nlohmann::json mbrRegion):
        m_pointLow{ mbrRegion["pointLow"] },
        m_pointHigh{ mbrRegion["pointHigh"] } {}

    Rectangle& Rectangle::operator=(Rectangle& other) {
        m_pointLow = other.m_pointLow;
        m_pointHigh = other.m_pointHigh;

        return *this;
    }

    Point Rectangle::pointLow() { return m_pointLow; }

    Point Rectangle::pointHigh() { return m_pointHigh; }

}