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
using json = nlohmann::json;


class Plane {
public:
    Plane(int key, int value): m_key{ key }, m_value{ value } {}

    Plane(uint8_t* buffer) {
        int offset{ 0 };

        std::memcpy(&m_key, &buffer[offset], sizeof(m_key));
        offset += sizeof(m_key);

        std::memcpy(&m_value, &buffer[offset], sizeof(m_value));
        offset += sizeof(m_value);
    }

    size_t fillBuffer(uint8_t* buffer) {
        size_t offset{ 0 };

        std::memcpy(&buffer[offset], &m_key, sizeof(m_key));
        offset += sizeof(m_key);

        std::memcpy(&buffer[offset], &m_value, sizeof(m_value));
        offset += sizeof(m_value);

        return offset;
    }

    int getKey() { return m_key; }

    int getValue() { return m_value; }

private:
    int m_key;
    int m_value;
};

int exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        Plane myRecord{ 1, 111 };
        uint8_t buffer[100];

        int myKey = myRecord.getKey();
        size_t buf_size = myRecord.fillBuffer(buffer);

        Dbt key(&myKey, static_cast<uint32_t>(sizeof(myKey)));
        Dbt value(&buffer, static_cast<uint32_t>(buf_size));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << myKey << "'" << std::endl;
            return 1;
        }

        Plane readRecord(reinterpret_cast<uint8_t*>(retVal.get_data()));

        std::cout << "key is " << readRecord.getKey() << std::endl;
        std::cout << "value is " << readRecord.getValue() << std::endl;
    }
    catch (DbException& e)
    {
        std::cerr << "Error opening database: " << dbFileName << "\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error opening database: " << dbFileName << "\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}



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

    dbResource.WriteKeyValuePair(planeOid, serialized, modb::WRITE_NODUPDATA);

    modb::Plane readRecord = dbResource.FindById(planeOid);

    std::cout << "key is " << readRecord.oid() << " \t" << "value is " << readRecord.mbrRectangle().pointLow().latitude() << "-" << readRecord.mbrRectangle().pointLow().latitude() << std::endl;


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