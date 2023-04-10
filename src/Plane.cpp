#include <modb/Plane.h>

using Json = nlohmann::json;

namespace modb {
    Plane::Plane():  
            m_oid(""),
            m_baseLocation (),
            m_mbrLocation (),
            m_mbrRectangle()
    {}
    Plane::Plane(std::string oid, Point baseLocation, Point mbrLocation, Rectangle mbrRectangle) :
        m_oid{ oid },  m_baseLocation{ baseLocation }, m_mbrLocation{ mbrLocation }, m_mbrRectangle{ mbrRectangle }{}

    std::string Plane::Oid() { return m_oid; }

    Plane::Plane(Json json) {
        std::cout << "json to data conversion" << std::endl;
        m_oid = json["oid"].get<std::string>();
        
        Json baseLocation  = json["baseLocation"];
        m_baseLocation.latitude = baseLocation["latitude"];
        m_baseLocation.longitude = baseLocation["longitude"];

        Json mbrLocation = json["probabilityRegion"];


        // m_mbrLocation.latitude = mbrLocation["latitude"].get<float>();
        // m_mbrLocation.longitude = mbrLocation["longitude"].get<float>();
        

        // std::stringstream & deneme;
        std::cout << json << std::endl;
        
  
    }

    bool Plane::Plane::SetJson(Json json) { // tehre will be second parameter about schema in the future
        std::cout << "json logic can be here" << std::endl;
        return true;
    }

    Point Plane::BaseLocation() { return m_baseLocation; }

    Point Plane::MbrLocation() { return m_mbrLocation; }


    Rectangle Plane::MbrRectangle() { return m_mbrRectangle; }
}
