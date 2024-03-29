#include <db_cxx.h>
#include <modb/Object.h>
#include <modb/DatabaseManager.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <signal.h>

void mainSIGINT(int param)
{
}

void executeQuery(std::string dbName, modb::Region queryRegion) {
    modb::DatabaseManager db{dbName, DB_BTREE, DB_READ_COMMITTED};

    auto [truePositives, falsePositives] = db.intersectionQuery(queryRegion);

    for (modb::Object& object : truePositives) {
        std::cout << object.id() << '\n';
    }

    std::unique_ptr<modb::Stats> stats = db.getStats();

    std::cerr << "number of all positives: " << stats->allPositives << std::endl;
    std::cerr << "number of false positives: " << stats->falsePositives << std::endl << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "usage:\n";
        std::cerr << "intersection_query <database name> <pointlow longitude> <pointlow latitude> <pointhigh longitude> <pointhigh latitude>" << std::endl;
        return -1;
    }

    std::string dbName = argv[1];

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
        executeQuery(dbName, queryRegion);
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
