#include <db_cxx.h>
#include <modb/Plane.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/serialization.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>

const std::string dbFileName{ "plane.db" };

int exampleLoad() {
    try
    {
        Db planeDB{ NULL, 0 };
        planeDB.set_error_stream(&std::cerr);
        planeDB.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        const modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, 1.1 };

        std::ostringstream outputStream{};
        boost::archive::binary_oarchive outputArchive(outputStream);

        outputArchive << plane;
        std::string outputBuffer{outputStream.str()};

        Dbt key(const_cast<char*>(plane.oid.c_str()), static_cast<uint32_t>(plane.oid.length() + 1));
        Dbt value(const_cast<char*>(outputBuffer.c_str()), static_cast<uint32_t>(outputBuffer.length() + 1));

        planeDB.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        planeDB.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << plane.oid << "'" << std::endl;
            return 1;
        }

        uint32_t tmp = retVal.get_size();

        std::string inputBuffer{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};
        std::istringstream inputStream{inputBuffer};
        boost::archive::binary_iarchive inputArchive{inputStream};

        modb::Plane readRecord;
        inputArchive >> readRecord;

        std::cout << "key is " << readRecord.oid << std::endl;
    }
    catch (DbException& e)
    {
        std::cerr << "Error opening database: " << dbFileName << "\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error opening database: " << dbFileName << "\n";
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    exampleLoad();
}