#include <db_cxx.h>

#include <modb/DatabaseResource.h>
#include <modb/Object.h>
#include <modb/DatabaseResource.inl>

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


    modb::DatabaseResource<modb::Object> dbResource{dbFileName, DB_BTREE};

    modb::Object object{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, {0.3, 0.3} } };;

    std::string objectOid = object.id();
    std::string serialized = dbResource.Serializer_().Serialize(object);

    dbResource.WriteKeyValuePair(objectOid, serialized, modb::WRITE_DEFAULT);

    modb::Object readRecord;
    
    dbResource.FindById(objectOid, &readRecord);

    std::cout << "key is " << readRecord.id() << " \t" << "value is " << readRecord.mbrRegion().pointLow().latitude() << "-" << readRecord.mbrRegion().pointLow().latitude() << std::endl;

    [[maybe_unused]] pid_t pid = apiCallStarter();

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
                continue;
            }

            // there will be two database in this system 
            // First : data that be in r-tree is primary db.
            // Second : data that be in continuosly peek api

            // convert line to json
            json data = json::parse(line);

            modb::Object parsedObject{data};

            std::cout << parsedObject.id() << '\n'
                << parsedObject.baseLocation().longitude() << '\n'
                << parsedObject.mbrRegion().pointLow().longitude() << "\n\n";


            // // convert json to Object format 
            // const std::string ObjectOid = Object.id();
            // std::string serialized = dbResource.Serializer_().Serialize(Object);

            // dbResource.WriteKeyValuePair(ObjectOid, serialized, modb::WRITE_NODUPDATA);

            // modb::Object readRecord = dbResource.FindById(ObjectOid);


            // serialize object


            // write berkeleydb


            i++;
        }

    }
    catch (std::exception& e) {
        std::cout << "hey " << std::endl;
    }

    return 0;
}