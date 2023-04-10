#ifndef PLANE_H
#define PLANE_H

#include <modb/Point.h>
#include <modb/DatabaseResource.h>
#include <modb/Rectangle.h>

#include <string>

using Json = nlohmann::json;

namespace modb {
    // class Rectangle {
    //     public:

    //         float m_width;
    //         float m_height;
        
    //         Rectangle() = default;
    //         Rectangle(Rectangle &other);



    //         ~Rectangle() = default;

    //         template <class Archieve>
    //         inline void serialize(Archieve& ar, unsigned int) {
    //             ar& m_width;
    //             ar& m_height;
    //         }
    // };

    class Plane : public modb::DataObject {
    public:
        Plane();
        Plane(Json);

        virtual bool SetJson(Json json);

        Plane(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, modb::Rectangle mbrRectangle);

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& m_oid;
            ar& m_baseLocation;
            ar& m_mbrLocation;
            ar& m_mbrWidth;
        }

        std::string Oid();

        modb::Point BaseLocation();

        modb::Point MbrLocation();

        modb::Rectangle Plane::MbrRectangle();

    private:
        std::string m_oid;

        modb::Point m_baseLocation;

        modb::Point m_mbrLocation;

        modb::Rectangle m_mbrRectangle;
    };
}

#endif