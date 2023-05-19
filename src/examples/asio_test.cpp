#include <modb/Object.h>
#include <modb/DatabaseResource.h>

#include <db_cxx.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <signal.h>
#include <thread>
#include <cstdio>

using boost::asio::ip::udp;

std::atomic<bool> exitProgram = false;

void SIGINT_handler(int param)
{
    exitProgram = true;
}

void udpServer(std::shared_ptr<modb::DatabaseResource> db, unsigned short port) {
    boost::asio::io_context io_context;
    udp::endpoint endpoint{udp::v4(), port};
    udp::socket socket{io_context, endpoint};

    while (!exitProgram)
    {
        boost::array<char, 100> recv_buf;
        udp::endpoint remote_endpoint;
        boost::system::error_code ignored_error;

        std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
        if (exitProgram) { break; }

        std::string message{recv_buf.begin(), len};

        int operation;
        std::sscanf(message.c_str(), "%d ", &operation);

        std::string reply{};

        if (operation == 1) {
            modb::Region queryRegion;
            std::sscanf(message.c_str(), "%d %lf %lf %lf %lf",
                &operation,
                &queryRegion.pointLow().longitude(),
                &queryRegion.pointLow().latitude(),
                &queryRegion.pointHigh().longitude(),
                &queryRegion.pointHigh().latitude()
            );

            std::vector<modb::Object> resultset = db->intersectionQuery(queryRegion);

            std::for_each(resultset.begin(), resultset.end(),
                [&](const modb::Object& object) {
                    reply.append(object.id() + '\n');
                }
            );
        }
        else {
            continue;
        }

        socket.send_to(boost::asio::buffer(reply), remote_endpoint, 0, ignored_error);
    }

    std::cerr << "\nserver thread: exiting...";
}

void loadData(std::shared_ptr<modb::DatabaseResource> db) {
    std::string line;

    while (!exitProgram)
    {
        std::getline(std::cin, line);

        if (line.empty()) {
            std::cerr << "waiting..." << std::endl;
            sleep(1);
            continue;
        }

        json data = json::parse(line);
        modb::Object parsedObject{data};

        db->putObject(parsedObject);
    }

    std::cerr << "\nloader thread: exiting...";
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage:\nasio_test <database-name> <port-number>" << std::endl;
        return -1;
    }

    signal(SIGINT, SIGINT_handler);

    try {
        const auto db = std::make_shared<modb::DatabaseResource>(std::string(argv[1]), DB_BTREE, DB_CREATE);

        std::thread loaderThread{loadData, db};
        std::thread serverThread{udpServer, db, static_cast<unsigned short>(std::stoi(argv[2]))};

        loaderThread.join();
        serverThread.join();

        std::cerr << "\nmodb: exiting..." << std::endl;
    }
    // catch (DbException& e)
    // {
    //     std::cerr << e.what() << std::endl;
    //     return 1;
    // }
    catch (std::exception& e)
    {
        std::cerr << "Error:\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

