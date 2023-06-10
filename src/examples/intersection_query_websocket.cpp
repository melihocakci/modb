#include <db_cxx.h>
#include <modb/Object.h>
#include <modb/DatabaseManager.h>
#include <modb/SendDataWS.h>
#include <modb/IntersectionSendOption.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <signal.h>

void mainSIGINT(int param)
{
}

void executeQuery(std::string dbName, modb::Region queryRegion) {
    modb::DatabaseManager db{dbName, DB_BTREE, DB_READ_COMMITTED};

    std::vector<modb::Object> resultset = db.intersectionQuery(queryRegion);

    for (modb::Object& object : resultset) {
        std::cout << object.id() << '\n';
    }

    std::unique_ptr<modb::Stats> stats = db.getStats();

    std::cerr << "number of all positives: " << stats->allPositives << std::endl;
    std::cerr << "number of false positives: " << stats->falsePositives << std::endl << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage:\ntest_modb <database name> " << std::endl;
        return -1;
    }
    std::string pipeLocation = "/tmp/wsPipe";
    modb::websocket::SendDataWS sendDataWs{"127.0.0.1", pipeLocation, 8083};

    modb::websocket::IntersectionSendOption sender(sendDataWs);

    std::ofstream& pipeWriter = sendDataWs.pipeWriter(); // pipe writer request done

    std::thread dataSendProcess = std::thread([&sender]() {
       sender.startDataSendProcess();
        // std::cout << "run to the hill" << a << std::endl;
    });


    std::unique_lock<std::mutex> lock(sendDataWs.pipeWriterLock);
    sendDataWs.cv.wait(lock, [&sendDataWs] { return sendDataWs.isPipeWriterFree.load(); });
    
    if (!pipeWriter.is_open()) {
        std::cerr << "Failed to open the named pipe for writing." << std::endl;
    }


    std::string dbName = argv[1];

    modb::Region queryRegion = {
        {
            {sender.firstPoint().latitude()},
            {sender.firstPoint().longitude()}
        },
        {
            {sender.secondPoint().latitude()},
            {sender.secondPoint().longitude()}
        }
    };

    signal(SIGINT, mainSIGINT);

    try {
        executeQuery(dbName, queryRegion);
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
