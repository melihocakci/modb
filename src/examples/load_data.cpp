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
        if (i % 100  == 0) {
            std::cerr << "reading line " << i << '\n';
        }

        char* ret = std::fgets(buffer, sizeof(buffer) - 1, fp);

        if (ret == NULL || exitProgram) {
            break;
        }

        std::string line{buffer};

        json data = json::parse(line);
        modb::Object parsedObject{data};

        db.putObject(parsedObject);
    }

    std::cerr << "loading finished\n\n";
    std::fclose(fp);

    auto [dbStats, idxStats] = db.getStats();

    std::cerr << "number of unique keys in b-tree: " << dbStats->bt_nkeys << std::endl;
    std::cerr << "number of data items in b-tree: " << dbStats->bt_ndata << std::endl;
    std::cerr << "level of b-tree: " << dbStats->bt_levels << std::endl << std::endl;

    std::cerr << "number of objects in r-tree: " << idxStats->getNumberOfData() << std::endl;
    std::cerr << "number of nodes in r-tree: " << idxStats->getNumberOfNodes() << std::endl;
    std::cerr << "number of nodes in r-tree: " << idxStats->getReads() << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "usage:\nload_file <input-file> <db-name> <record-number> <mbr-size>" << std::endl;
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
