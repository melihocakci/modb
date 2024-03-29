#pragma once

#include <modb/SendDataWS.h>
#include <modb/SendOption.h>
#include <modb/Point.h>

namespace modb
{
    namespace websocket {

        class IntersectionSendOption : public SendOption // maybe inherited fron SendDataWS
        {
            private:
                SendDataWS& m_sendDataWS;
                modb::Point m_firstPoint;
                modb::Point m_secondPoint;
            public:
                IntersectionSendOption(SendDataWS&) ;
                modb::Point firstPoint(); 
                modb::Point secondPoint(); 
                void startDataSendProcess();
                void buildOption() override;
                void buildOption (modb::Point, modb::Point);
                
        };

    }
}
