#include <db_cxx.h>
#include <modb/Object.h>
#include <modb/DatabaseManager.h>
#include <modb/SendDataWS.h>

#include <iostream>
#include <string>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <signal.h>
#include <tuple>
#include <thread>


using nlohmann::json;

constexpr std::size_t buffsize = 100;
bool exitProgram = false;

void mainSIGINT(int param)
{
    exitProgram = true;
}

void load_data( const std::string dbName ) {


    modb::DatabaseManager db{dbName, DB_BTREE, DB_CREATE, -1}; // mbr size is not required cohesion problem
    std::string pipeLocation = "/tmp/wsPipe";
    
    
    modb::websocket::SendDataWS sendDataWs{"127.0.0.1", pipeLocation, 8083};

    // std::ofstream deneme{"/tmp/wsPipe"};

    // deneme << "onur " << std::endl;

    std::ofstream& pipeWriter = sendDataWs.pipeWriter(); // pipe writer request done

    // int a = 5;
    std::thread dataSendProcess = std::thread([&sendDataWs]() {
       sendDataWs.startDataSendProcess(modb::websocket::SendOption::SnapShot);
        // std::cout << "run to the hill" << a << std::endl;
    });
    
    std::unique_lock<std::mutex> lock(sendDataWs.pipeWriterLock);
    sendDataWs.cv.wait(lock, [&sendDataWs] { return sendDataWs.isPipeWriterFree.load(); });
    
    if (!pipeWriter.is_open()) {
        std::cerr << "Failed to open the named pipe for writing." << std::endl;
    }
    

    db.forEach([&pipeWriter](const modb::Object& object) {
        // Write to the pipe
        std::stringstream ss;

        ss << "point," << object.id() << "," << object.baseLocation().latitude() << "," << object.baseLocation().longitude() << std::endl;
        pipeWriter << ss.str();
    });
    
    try
    {
        /* code */
        sendDataWs.isJoined.store(true);
        dataSendProcess.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage:\nload_bdb_websocket <db-name>" << std::endl;
        return -1;
    }

    signal(SIGINT, mainSIGINT);

    try {
        load_data(argv[1]);
    }
    catch (DbException& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
