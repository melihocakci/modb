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

        std::ostringstream outputStream{};
        boost::archive::binary_oarchive outputArchive{outputStream};

        std::string ObjectOid = parsedObject.id();

        outputArchive << parsedObject;
        std::string serialized{outputStream.str()};

        Dbt key(const_cast<char*>(ObjectOid.data()), static_cast<uint32_t>(ObjectOid.length()));
        Dbt value(const_cast<char*>(serialized.data()), static_cast<uint32_t>(serialized.length()));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << ObjectOid << "'" << std::endl;
        }

        std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

        std::istringstream inputStream{newObject};
        boost::archive::binary_iarchive inputArchive{inputStream};

        modb::Object readRecord;

        inputArchive >> readRecord;

        std::cout << "read key: " << readRecord.id() << "\n\n";
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
