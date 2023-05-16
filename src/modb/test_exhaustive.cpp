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
#include <signal.h>

std::string dbFileName;
bool exitProgram = false;

void siginthandler(int param)
{
    exitProgram = true;
}

void exhaustive(modb::Region queryRegion) {
    Db db(nullptr, 0);
    db.open(nullptr, (dbFileName + ".db").c_str(), nullptr, DB_BTREE, DB_CREATE, 0);

    Dbc* cursor;
    db.cursor(nullptr, &cursor, 0);

    Dbt key, data;
    int counter = 0;
    while (cursor->get(&key, &data, DB_NEXT) == 0 && !exitProgram) {
        // Retrieve key and data values
        char* keyStr = static_cast<char*>(key.get_data());
        int keySize = key.get_size();

        char* dataStr = static_cast<char*>(data.get_data());
        int dataSize = data.get_size();

        // Process the key and data
        std::cout << "Key: " << std::string(keyStr, keySize) << std::endl;
        std::cout << "Data: " << std::string(dataStr, dataSize) << std::endl;

        // Release key and data resources
        key.set_data(nullptr);
        data.set_data(nullptr);
        ++counter;
    }

    cursor->close();
    db.close(0);
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "usage:\ntest_modb <database name> <pointlow longitude> <pointlow latitude> <pointhigh longitude> <pointhigh latitude>" << std::endl;
        return -1;
    }

    dbFileName = argv[1];

    modb::Region queryRegion = {
        {
            {std::stod(argv[2])},
            {std::stod(argv[3])}
        },
        {
            {std::stod(argv[4])},
            {std::stod(argv[5])}
        }
    };

    signal(SIGINT, siginthandler);
    try {
        exhaustive(queryRegion);
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
