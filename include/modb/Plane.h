#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>

#include <string>
#include <cstdint>

namespace modb {
    class Plane {
    public:
        Plane() = delete;

        Plane(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, double mbrWidth);

        Plane(uint8_t* buffer);

        ~Plane() = default;

        size_t fillBuffer(uint8_t* buffer);

        std::string getOid();

    private:
        template <typename T> void putInBuffer(T* object, uint8_t* buffer, size_t& offset);

        template <typename T> void getFromBuffer(T* object, uint8_t* buffer, size_t& offset);

        std::string m_oid;

        modb::Point m_baseLocation;

        modb::Point m_mbrLocation;

        double m_mbrWidth;
    };
}

#endif