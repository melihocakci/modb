#ifndef OBJECT_H
#define OBJECT_H

#include <modb/Point.h>
#include <modb/Region.h>

#include <nlohmann/json.hpp>

#include <string>

namespace modb {

    class Object {
    public:
        Object();

        Object(const Object& other);

        Object(Object&& other);

        Object(std::string id, Point baseLocation, Region mbrRegion);

        Object(const nlohmann::json& json);

        Object& operator=(const Object& other);

        virtual ~Object() = default;

        template <class Archive>
        void serialize(Archive& ar, unsigned int) {
            ar& m_oid;
            ar& m_baseLocation;
            ar& m_mbrRegion;
        }

        std::string& id();
        const std::string& id() const;

        Point& baseLocation();
        const Point& baseLocation() const;

        Region& mbrRegion();
        const Region& mbrRegion() const;

    private:
        std::string m_oid;
        Point m_baseLocation;
        Region m_mbrRegion;
    };
}

#endif
