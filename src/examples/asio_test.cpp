#include <modb/DatabaseResource.h>

#include <boost/asio/placeholders.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>

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
using nlohmann::json;

std::atomic<bool> exitProgram = false;
boost::asio::io_context io_context;

void SIGINT_handler(int param)
{
    exitProgram = true;
    io_context.stop();
}

class udp_server
{
public:
    udp_server(boost::asio::io_context& io_context, unsigned short port, std::shared_ptr<modb::DatabaseResource> db) :
        socket_(io_context, udp::endpoint(udp::v4(), port)),
        db_(db)
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_),
            remote_endpoint_,
            boost::bind(&udp_server::handle_receive,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
        std::size_t bytes_transferred)
    {
        if (error) {
            return;
        }

        std::string message{recv_buffer_.begin(), bytes_transferred};

        // int operation;
        // std::sscanf(message.c_str(), "%d ", &operation);

        auto reply = std::make_shared<std::string>();

        // if (operation == 1) {
        modb::Region queryRegion;
        std::sscanf(message.c_str(), "%lf %lf %lf %lf",
            &queryRegion.pointLow().longitude(),
            &queryRegion.pointLow().latitude(),
            &queryRegion.pointHigh().longitude(),
            &queryRegion.pointHigh().latitude()
        );

        std::vector<modb::Object> resultset = db_->intersectionQuery(queryRegion);

        std::for_each(resultset.begin(), resultset.end(),
            [&](const modb::Object& object) {
                reply->append(object.id() + '\n');
            }
        );

        if (reply->empty()) {
            reply->append("\n");
        }

        socket_.async_send_to(boost::asio::buffer(*(reply.get())),
            remote_endpoint_,
            boost::bind(&udp_server::handle_send, this,
                boost::asio::placeholders::error));

        // }

        start_receive();
    }

    void handle_send(const boost::system::error_code& error)
    {
        if (error) {
            std::cerr << "error\n";
            SIGINT_handler(0);
        }
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 100> recv_buffer_;
    std::shared_ptr<modb::DatabaseResource> db_;
};

void udpServer(std::shared_ptr<modb::DatabaseResource> db, unsigned short port) {
    udp_server server(io_context, port, db);
    io_context.run();
}

void dataLoader(std::shared_ptr<modb::DatabaseResource> db) {
    std::string line;

    while (!exitProgram)
    {
        std::getline(std::cin, line);

        if (line.empty()) {
            std::cerr << "modb: waiting..." << std::endl;
            sleep(1);
            continue;
        }

        json data = json::parse(line);
        modb::Object newObject{data};

        db->putObject(newObject);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage:\nasio_test <database-name> <port-number>" << std::endl;
        return -1;
    }

    signal(SIGINT, SIGINT_handler);

    try {
        const auto db = std::make_shared<modb::DatabaseResource>(std::string(argv[1]), DB_BTREE, DB_CREATE);

        std::thread loaderThread{dataLoader, db};
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

