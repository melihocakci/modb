#ifndef SENDDATAWS_H
#define SENDDATAWS_H

#include <boost/beast/core.hpp> 
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <atomic>

#include <condition_variable>

namespace modb
{

    namespace websocket {

        enum SendOption {
            SnapShot, // send all database to socket 
            Intersection, // send interval queries to web
            Tree // see mbr and tree structure maybe this is same functionality with snapshot
        };
        class SendDataWS {
        public:
            std::atomic<bool> isPipeWriterFree;
            std::atomic<bool> isJoined;

            std::mutex pipeWriterLock;
            std::condition_variable cv;

            SendDataWS();
            SendDataWS(const std::string& address, const std::string& pipeLocation, const int& port);
            // SendDataWS(const SendDataWS& other);
            // SendDataWS(SendDataWS&& other);
            ~SendDataWS();
            void startDataSendProcess(SendOption option);
            std::ofstream& pipeWriter();
        private:
            const std::string& m_address;
            const std::string& m_pipeLocation;
            const std::atomic<int> m_port;
            std::thread m_openedPipeThread;
            // std::ofstream m_pipeFile;
            std::ofstream m_pipeWriter;




        };
    }

}
#endif