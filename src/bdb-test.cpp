#include <db_cxx.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <cstring>


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

void apiCallStarter() {
    std::string filename = "api_call/opensky_test.py";
    std::string command = "python3 ";
    command += filename;

    pid_t c_pid = fork();

    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);

    }
    else if (c_pid > 0) {
        std::cout << "printed from parent process" << getpid() << std::endl;
    }
    else {
        system(command.c_str());
        std::cout << "printed from child process" << getpid() << std::endl;
    }

    // fscanf(in, "%s");

    // pclose(in);
}

int main(int argc, char** argv) {
    apiCallStarter();
    std::string line;


    int i = 0;

    std::ifstream f("/home/onur/moving-object-db-system/appsetting.json");
    json data = json::parse(f);

    std::cout << "if it is called twice" << std::endl;
    std::ifstream file{data["pipePath"]};
    while(i < 1000) {
        std::getline(file, line);
        std::cout << line << '\n';
        i++;
    }
    return 0;
}