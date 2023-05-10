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
    std::string line;
    modb::DatabaseResource db{"plane.db", DB_BTREE};

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

        db.updateObject(parsedObject);

        modb::Object newObject;
        db.getObject(parsedObject.id(), newObject);

        std::cout << "read key: " << newObject.id() << '\n'
            << "read baseLocation: " << newObject.baseLocation().longitude()
            << " - " << newObject.baseLocation().latitude() << "\n\n";
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
