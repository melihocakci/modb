
#include <modb/IntersectionSendOption.h>
#include <modb/SendOption.h>
#include <modb/SendDataWS.h>


using namespace modb::websocket;

IntersectionSendOption::IntersectionSendOption(SendDataWS& ws) :  m_sendDataWS{ws}, m_firstPoint{}, m_secondPoint{} { }

void IntersectionSendOption::startDataSendProcess() {
    modb::websocket::SendOption* sendOptionPtr = dynamic_cast<modb::websocket::SendOption*>(this);
    m_sendDataWS.startDataSendProcess(modb::websocket::SendOptionEnum::Intersection, sendOptionPtr);

// m_sendDataWS.startDataSendProcess(modb::websocket::SendOptionEnum::Intersection, const_cast<modb::websocket::SendOption*>(static_cast<const modb::websocket::SendOption*>(this)));
}

modb::Point IntersectionSendOption::firstPoint() {
    return m_firstPoint;
}

modb::Point IntersectionSendOption::secondPoint() {
    return m_secondPoint;
}

void IntersectionSendOption::buildOption() {
    std::cout << "" << std::endl;
}

// TODO : challenge make this generic and all Args directly load inherited class later
void IntersectionSendOption::buildOption (modb::Point firstPoint, modb::Point secondPoint) {
    m_firstPoint = std::move(firstPoint);
    m_secondPoint = std::move(secondPoint);
}

