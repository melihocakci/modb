#include <db_cxx.h>
#include <modb/Object.h>
#include <modb/DatabaseResource.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>

const std::string dbFileName{ "modb.db" };

void readObjects() {

    Db myDb{ NULL, 0 };
    myDb.set_error_stream(&std::cerr);
    myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

    std::string line;

    while (true) {
        std::getline(std::cin, line);

        if (line.empty()) {
            std::cerr << "waiting..." << std::endl;
            sleep(1);
            continue;
        }
        json data = json::parse(line);

        modb::Object parsedObject{data};
        std::cout << "parsed key: " << parsedObject.id() << '\n';

        modb::DatabaseResource db{"plane.db", DB_BTREE};

        int ret = db.putObject(parsedObject);

        if (ret) {
            std::cerr << "modb: Record insertion failed" << std::endl;
            throw std::exception{};
        }

        modb::Object newObject; 
        ret = db.getObject(parsedObject.id(), newObject);

        if (ret) {
            std::cerr << "modb: Record retreaval failed" << std::endl;
            throw std::exception{};
        }

        std::cout << "read key: " << newObject.id() << "\n\n";
    }
}

int main(int argc, char** argv) {
    try {
        readObjects();
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
