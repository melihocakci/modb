#ifndef ATOMICDATATYPES_H
#define ATOMICDATATYPES_H
// atomic class for DataResource.<your generic datatype like int>.cpp

#include "CommonMacros.h"
#include <string>
#include <nlohmann/json.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>


using Json = nlohmann::json;


namespace modb::atomic {
    
    class safe_int: public DataObject {
    public:
        safe_int()=default;

        // safe_int(Json);

        // safe_int(safe_int& other);

        // safe_int(std::string oid, Point baseLocation, Rectangle mbrRectangle);

        // safe_int& operator=(safe_int& other);

        virtual ~safe_int() = default;

        virtual bool SetJson(Json json){
            std::string data = json["data"];
            if(data.empty()) {
                return false;
            }

            data = json["data"].get<int>();
            return true;
        }; 

        template <class Archive>
        inline void serialize(Archive& ar, unsigned int) {
            ar& data;
        }
        int data;
    };

    class safe_float : public DataObject 
    {
        public:

        safe_float() = default;


        virtual ~safe_float() = default;

        // virtual bool SetJson(Json json);

        safe_float(safe_float& other) = default;

        template <class Archive> 
        inline void serialize(Archive& ar, unsigned int) { 
            ar& data;
        }


        float data;

    };

    class safe_double : public DataObject 
    {
        public:

        safe_double() = default;


        virtual ~safe_double() = default;

        // virtual bool SetJson(Json json);

        safe_double(safe_double& other) = default;

        template <class Archive> 
        inline void serialize(Archive& ar, unsigned int) { 
            ar& data;
        }

        double data;

    };

    class safe_string : public DataObject 
    {
        public:
        safe_string() = default;


        virtual ~safe_string() = default;

        // virtual bool SetJson(Json json);

        safe_string(safe_string& other) = default;

        template <class Archive> 
        inline void serialize(Archive& ar, unsigned int) { 
            ar& data;
        }

        std::string data;
    };


}

#endif