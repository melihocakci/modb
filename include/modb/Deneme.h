#ifndef EXAMPLE_H
#define EXAMPLE_H
#include <modb/Point.h>
#include <string>

namespace modb 
{
    class Example {
        public:
            Example() = default;

            ~Example() = default;

            Example(std::string oid, modb::Point baseLocation, modb::Point mbrLocation, float mbrWidth);
            template <class Archive>
            inline void serialize(Archive& ar, unsigned int) {
                ar& m_oid;
                ar& m_baseLocation;
                ar& m_mbrLocation;
                ar& m_mbrWidth;
            }
            std::string getOid();

            modb::Point getBaseLocation();

            modb::Point getMbrLocation();

            float getMbrWidth();

        private:
            char * code;
    };

}

#endif