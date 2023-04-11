#ifndef OBJECT_H
#define OBJECT_H

#include <modb/Point.h>
#include <modb/DatabaseResource.h>
#include <modb/Rectangle.h>

#include <nlohmann/json.hpp>

#include <string>

using nlohmann::json;

namespace modb {

    class Object: public DataObject {
    public:
        Object();

        Object(json json);

        Object(Object& other);

        Object(std::string id, Point baseLocation, Rectangle mbrRectangle);

        Object& operator=(Object& other);

        virtual ~Object() = default;

        virtual bool SetJson(json json);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_oid;
            ar& m_baseLocation;
            ar& m_mbrRegion;
        }

        std::string id();

        Point baseLocation();

        Rectangle mbrRegion();

    private:
        std::string m_oid;
        Point m_baseLocation;
        Rectangle m_mbrRegion;
    };
}

#endif