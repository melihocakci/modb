#include <modb/Object.h>
#include <modb/DatabaseResource.h>
#include <modb/Common.h>

#include <db_cxx.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <signal.h>

std::string dbFileName;

void mainSIGINT(int param)
{
}

void exhaustive(const modb::Region& queryRegion) {
    modb::DatabaseResource db{dbFileName, DB_BTREE, DB_READ_COMMITTED};

    db.forEach(
        [&](const modb::Object& object) {
            if (pointWithinRegion(object.baseLocation(), queryRegion)) {
                std::cout << object.id() << '\n';
            }
        }
    );
}

int main(int argc, char** argv) {
    if (argc != 6) {
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

    signal(SIGINT, mainSIGINT);

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
