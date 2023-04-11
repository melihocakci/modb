#include <db_cxx.h>

#include <modb/DatabaseResource.h>
#include <modb/Plane.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <cstring>

#include <sys/wait.h>

const std::string dbFileName{ "test.db" };

#include <nlohmann/json.hpp>
using nlohmann::json;

pid_t apiCallStarter() {
    std::string filename = "api_call/opensky_test.py";
    std::string command = "python3";

    pid_t c_pid = fork();

    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);

    }
    else if (c_pid > 0) {
        // waitpid(c_pid, NULL, WUNTRACED);
        std::cout << "printed from parent process" << getpid() << std::endl;
        // raise(SIGSTOP);
    }
    else {
        execlp(command.c_str(), command.c_str(), filename.c_str(), NULL);
        std::cout << "printed from child process" << getpid() << std::endl;

        raise(SIGSTOP);
    }

    return c_pid;
}

void BusyWaiting() {
    int i = 0;

    while (i < 1000000000) {
        i++;
    }

}

int main(int argc, char** argv) {
    // modb::DatabaseResource resource;


    modb::DatabaseResource<modb::Plane> dbResource{dbFileName, DB_BTREE};

    modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, {0.3, 0.3} } };;

    const std::string planeOid = plane.oid();
    std::string serialized = dbResource.Serializer_().Serialize(plane);

    dbResource.WriteKeyValuePair(planeOid, serialized, modb::WRITE_DEFAULT);

    modb::Plane readRecord = dbResource.FindById(planeOid);

    std::cout << "key is " << readRecord.oid() << " \t" << "value is " << readRecord.mbrRegion().pointLow().latitude() << "-" << readRecord.mbrRegion().pointLow().latitude() << std::endl;


    pid_t pid = apiCallStarter();


    std::string line;

    int i = 0;

    sleep(1);

    std::ifstream f("../appsetting.json"); // this place is in /etc while building production code
    json appSetting = json::parse(f);

    std::cout << "Hey main Program Working" << std::endl;
    std::ifstream file{appSetting["pipePath"]};

    try {
        while (i < 1000) {
            std::getline(file, line);
            if (line.empty()) {
                // BusyWaiting();
                std::cout << "pipe şuan boştur" << std::endl;
                continue;
            }

            // there will be two database in this system 
            // First : data that be in r-tree is primary db.
            // Second : data that be in continuosly peek api

            // convert line to json
            json data = json::parse(line);


            std::string value = data["oid"].get<std::string>();
            

            // std::stringstream ss;
            // ss << data["oid"] ;
            // std::string deneme = ss.str();

            modb::Plane record{data};

            // convert json to Plane format 
            const std::string planeOid = plane.oid();
            std::string serialized = dbResource.Serializer_().Serialize(plane);

            dbResource.WriteKeyValuePair(planeOid, serialized, modb::WRITE_NODUPDATA);

            modb::Plane readRecord = dbResource.FindById(planeOid);

            std::cout << "key is " << readRecord.oid() << " \t" << "value is " << readRecord.baseLocation().longitude() << readRecord.baseLocation().latitude() << std::endl;

            // serialize object


            // write berkeleydb



            std::cout << line << '\n';

            i++;
        }

    }
    catch (std::exception& e) {
        std::cout << "hey " << std::endl;
    }

    return 0;
}