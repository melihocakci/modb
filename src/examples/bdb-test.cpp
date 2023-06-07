#include <db_cxx.h>

#include <modb/MyQueryStrategy.h>
#include <modb/IndexManager.h>
#include <modb/DatabaseManager.h>
#include <modb/Object.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <cstring>

#include <sys/wait.h>

// // Library effective with Windows
// #include <windows.h>

// Library effective with Linux
#include <unistd.h>

const std::string dbFileName{ "test.db" };

using nlohmann::json;

bool isRedisSet = false;

pid_t apiCallStarter() {

    std::string filename = "api_call/opensky_test.py";
    if (isRedisSet == true)
    {
        std::stringstream ss;
        ss << "api_call/opensky_test_" << "redis" << ".py";
        std::string filename = ss.str();

    }

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


void clearRedisCacheInSystem() {
    system("redis-cli flushall");
}

int main(int argc, char** argv) {

    modb::DatabaseManager dbResource{dbFileName, DB_BTREE};
    modb::IndexManager IndexManager{dbFileName, dbResource};

    // MyQueryStrategy queryStrategy;

    // IndexManager.setQueryStrategy(queryStrategy);


    // modb::Object object{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, {0.3, 0.3} } };;
    // dbResource.putObject(object);
    // modb::Object readRecord;
    // dbResource.getObject(object.id(), readRecord);
    // std::cout << "key is " << readRecord.id() << " \t" << "value is " << readRecord.mbrRegion().pointLow().latitude() << "-" << readRecord.mbrRegion().pointLow().latitude() << std::endl;

    [[maybe_unused]] pid_t pid = apiCallStarter();

    std::string line;

    int i = 0;

    sleep(1);

    std::ifstream f("../appsetting.json"); // this place is in /etc while building production code
    json appSetting = json::parse(f);

    std::cout << "Hey main Program Working" << std::endl;
    std::ifstream file{appSetting["pipePath"]};

    try {
        while (true) {
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
                << "location : "
                << parsedObject.baseLocation().longitude() << '\t'
                << parsedObject.baseLocation().latitude() << '\n'
                << "mbrRegion:"
                << parsedObject.mbrRegion().pointLow().longitude() << "\t"
                << parsedObject.mbrRegion().pointLow().latitude() << "\n\t"
                << parsedObject.mbrRegion().pointHigh().longitude() << "\t";


            bool isQuickReturn = IndexManager.evaluateObject(parsedObject); // if it is indexed, evaluated object is written.

            std::cout << isQuickReturn << std::endl;

            if (isQuickReturn) {
                std::cout << "mbr already there." << std::endl;
            }

            dbResource.putObject(parsedObject);

            sleep(0.1);
            i++;
        }

        // clear this if there is no redis cli
        if (isRedisSet)
            clearRedisCacheInSystem();
    }
    catch (std::exception& e) {
        std::cout << "hey " << std::endl;
    }

    return 0;
}