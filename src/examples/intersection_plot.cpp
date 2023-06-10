#include <db_cxx.h>
#include <modb/Object.h>
#include <modb/DatabaseManager.h>
#include <gnuplot-iostream.h>

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



    gnuplotio::Gnuplot gp;

    gp << "set xrange [" << queryRegion.pointLow().longitude() - 0.5 << ":" << queryRegion.pointHigh().longitude() + 0.5 << "]\n"
        << "set yrange [" << queryRegion.pointLow().latitude() - 0.5 << ":" << queryRegion.pointHigh().latitude() + 0.5 << "]\n"
        << "set terminal png size 3840, 2160\n"
        << "set output 'output.png'\n";

    int counter = 1;

    modb::Region plotRegion{};

    gp << "set object " << counter++
        << " rectangle from "
        << queryRegion.pointLow().longitude() << "," << queryRegion.pointLow().latitude()
        << " to " << queryRegion.pointHigh().longitude() << "," << queryRegion.pointHigh().latitude()
        << " fillstyle empty border lc rgb 'black' lw 3\n";

    for (modb::Object& object : truePositives) {
        gp << "set object " << counter++
            << " rectangle from "
            << object.mbrRegion().pointLow().longitude() << "," << object.mbrRegion().pointLow().latitude()
            << " to " << object.mbrRegion().pointHigh().longitude() << "," << object.mbrRegion().pointHigh().latitude()
            << " fillstyle empty border lc rgb 'blue' lw 2\n";

        gp << "set object " << counter++
            << " circle at "
            << object.baseLocation().longitude() << "," << object.baseLocation().latitude()
            << " size 0.01 fillstyle solid fillcolor rgb 'blue' lw 2\n";
    }

    for (modb::Object& object : falsePositives) {
        gp << "set object " << counter++
            << " rectangle from "
            << object.mbrRegion().pointLow().longitude() << "," << object.mbrRegion().pointLow().latitude()
            << " to " << object.mbrRegion().pointHigh().longitude() << "," << object.mbrRegion().pointHigh().latitude()
            << " fillstyle empty border lc rgb 'red' lw 2\n";

        gp << "set object " << counter++
            << " circle at "
            << object.baseLocation().longitude() << "," << object.baseLocation().latitude()
            << " size 0.01 fillstyle solid fillcolor rgb 'red' lw 2\n";
    }

    gp << "plot 0 notitle\n";
}

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "usage:\n";
        std::cerr << "intersection_plot <database name> <pointlow longitude> <pointlow latitude> <pointhigh longitude> <pointhigh latitude>" << std::endl;
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
