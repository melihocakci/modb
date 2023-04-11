#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>
#include <modb/DatabaseResource.h>
#include <modb/Rectangle.h>

#include <nlohmann/json.hpp>

#include <string>

using Json = nlohmann::json;

namespace modb {

    class Plane: public DataObject {
    public:
        std::string oid;
        Point baseLocation;
        Point mbrLocation;
        float mbrWidth;

        Plane();

        Plane(Json);

        Plane(Plane& other);

        Plane(std::string oid, Point baseLocation, Rectangle mbrRectangle);

        Plane& operator=(Plane& other);

        virtual ~Plane() = default;

        virtual bool SetJson(Json json);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_oid;
            ar& m_baseLocation;
            ar& m_mbrRegion;
        }

        std::string oid();

        Point baseLocation();

        Rectangle mbrRegion();

    private:
        std::string m_oid;
        Point m_baseLocation;
        Rectangle m_mbrRegion;
    };
}

#endif