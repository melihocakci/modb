#include <db_cxx.h>

#include <iostream>
#include <string>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;



const char* dbFileName{ "test.db" };

class Record {
public:
    Record(int key, int value): m_key{ key }, m_value{ value } {}

    int getKey() { return m_key; }

    int getValue() { return m_value; }

private:
    int m_key;
    int m_value;
};

bool exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName, NULL, DB_BTREE, DB_CREATE, 0);

        Record myRecord{ 1, 111 };
        auto myKey = myRecord.getKey();

        Dbt key(reinterpret_cast<void*>(&myKey), static_cast<uint32_t>(sizeof(myKey)));
        Dbt value(reinterpret_cast<void*>(&myRecord), static_cast<uint32_t>(sizeof(myRecord)));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << myKey << "'" << std::endl;
            return 1;
        }

        Record* readRecord = reinterpret_cast<Record*>(retVal.get_data());

        std::cout << "key is " << readRecord->getKey() << std::endl;
        std::cout << "value is " << readRecord->getValue() << std::endl;
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
    return true;
}

void apiCallStarter () {
    std::string filename = "api_call/opensky_test.py";
    std::string command = "python3 ";
    command += filename;

    pid_t c_pid = fork();

    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);

    } else if (c_pid > 0) {
        std::cout << "printed from parent process" << getpid() << std::endl;
    } else {
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