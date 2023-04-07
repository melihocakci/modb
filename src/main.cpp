#include <db_cxx.h>
#include <modb/Plane.h>

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <memory>

const std::string dbFileName{ "plane.db" };

int exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, 1.1 };
        std::string planeOid = plane.getOid();

        
        std::unique_ptr<uint8_t> buffer = std::make_unique<uint8_t>(100);
        size_t buf_size = plane.fillBuffer(buffer.get());

        Dbt key(&planeOid, static_cast<uint32_t>(sizeof(planeOid)));
        Dbt value(buffer.get(), static_cast<uint32_t>(buf_size));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << planeOid << "'" << std::endl;
            return 1;
        }

        modb::Plane readRecord(reinterpret_cast<uint8_t*>(retVal.get_data()));

        std::cout << "key is " << readRecord.getOid() << std::endl;
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