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
#include <tuple>

#define INSERT 1
#define QUERY 2

using nlohmann::json;

constexpr std::size_t buffsize = 100;
bool exitProgram = false;

void mainSIGINT(int param)
{
    exitProgram = true;
}

void load_data(const std::string inputFile, const std::string dbName, int lineNum, double mbrSize) {
    char buffer[buffsize];
    FILE* fp = std::fopen(inputFile.c_str(), "r");

    if (fp == NULL) {
        std::cerr << "Did you mean " << inputFile.c_str() << " is maybe incorrect" << std::endl;
        std::cerr << "Please check your file name" << std::endl;
        return;
    }

    modb::DatabaseResource db{dbName, DB_BTREE, DB_CREATE, mbrSize};

    for (int i = 0; i < lineNum; i++) {
        if (i % 1000 == 0) {
            std::cerr << "loading record " << i << '\n';
        }

        char* ret = std::fgets(buffer, sizeof(buffer) - 1, fp);

        if (ret == NULL || exitProgram) {
            break;
        }

        std::string line{buffer};
        std::stringstream ss{line};

        int operation;
        ss >> operation;

        if (operation == INSERT) {
            std::string id;
            double longitude, latitude;

            ss >> id >> longitude >> latitude;
            modb::Object newObject{id, { longitude, latitude }};

            db.putObject(newObject);
        }
        else if (operation == QUERY) {
            double lowlong, lowlat, highlong, highlat;

            ss >> lowlong >> lowlat >> highlong >> highlat;

            [[maybe_unused]] std::vector resultset = db.intersectionQuery({ {lowlong, lowlat}, {highlong, highlat} });

            // std::cerr << "query region: " << lowlong << ' ' << lowlat << ' ' << highlong << ' ' << highlat << '\n';
            // std::cerr << "resultset size: " << resultset.size() << '\n';
        }

    }

    std::cerr << "finished loading " << lineNum << " records\n\n";
    std::fclose(fp);

    std::unique_ptr<modb::Stats> stats = db.getStats();

    std::cerr << "number of unique keys in b-tree: " << stats->dbStats->bt_nkeys << std::endl;
    std::cerr << "number of data items in b-tree: " << stats->dbStats->bt_ndata << std::endl;
    std::cerr << "number of pages in b-tree: " << stats->dbStats->bt_pagecnt << std::endl;
    std::cerr << "tree level of b-tree: " << stats->dbStats->bt_levels << std::endl << std::endl;

    std::cerr << "number of objects in r-tree: " << stats->idxStats->getNumberOfData() << std::endl;
    std::cerr << "number of nodes in r-tree: " << stats->idxStats->getNumberOfNodes() << std::endl << std::endl;

    std::cerr << "number of updates to b-tree: " << stats->dbUpdates << std::endl;
    std::cerr << "number of updates to r-tree: " << stats->idxUpdates << std::endl << std::endl;

    std::cerr << "number of all positives: " << stats->allPositives << std::endl;
    std::cerr << "number of false positives: " << stats->falsePositives << std::endl << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "usage:\nload_query <input-file> <db-name> <record-number> <mbr-size>" << std::endl;
        return -1;
    }

    signal(SIGINT, mainSIGINT);

    try {
        load_data(argv[1], argv[2], std::stoi(argv[3]), std::stod(argv[4]));
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
