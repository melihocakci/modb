#ifndef SENDOPTION_H
#define SENDOPTION_H

#include <iostream>

namespace modb
{
    namespace websocket
    {
        // All Send option decorators under the send option
        class SendOption {
            public:

                // virtual ~SendOption() {}
            virtual void buildOption() {
                std::cout << "lastly building" << std::endl;
            }
            // template<typename... Args> // TODO : think later
            // void buildOption(Args... args) {
            //     std::cout << "this build used by Decorator" << std::endl;
            // }
            // // Point(const Point& other);
        };
    }
}

#endif