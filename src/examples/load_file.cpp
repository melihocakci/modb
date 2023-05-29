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

constexpr std::size_t buffsize = 100;
bool exitProgram = false;

void mainSIGINT(int param)
{
    exitProgram = true;
}

void load_data(const std::string input, const std::string output) {
    char buffer[buffsize];
    FILE* fp = std::fopen(input.c_str(), "r");

    modb::DatabaseResource db{output, DB_BTREE, DB_CREATE};

    int counter = 0;

    while (!exitProgram) {
        char* ret = std::fgets(buffer, sizeof(buffer), fp);

        if (ret == NULL) {
            std::cerr << "modb: loading finished\n";
            break;
        }


        std::string line{buffer};

        json data = json::parse(line);
        modb::Object parsedObject{data};

        std::cerr << "line: " << counter++ << '\n';

        // std::cout << "parsed key: " << parsedObject.id() << '\n';

        db.putObject(parsedObject);

        // std::cout << "read key: " << newObject.id() << '\n'
        //     << "read baseLocation: " << newObject.baseLocation().longitude()
        //     << " - " << newObject.baseLocation().latitude() << "\n\n";
    }

    std::fclose(fp);
    std::cerr << "modb: exiting...\n";
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage:\nasio_test <input> <output>" << std::endl;
        return -1;
    }

    signal(SIGINT, mainSIGINT);

    try {
        load_data(std::string{argv[1]}, std::string{argv[2]});
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
