#include <db_cxx.h>
#include <modb/Plane.h>
#include <modb/DatabaseResource.h>


#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdint>
#include <memory>
#include <sstream>

const std::string dbFileName{ "plane.db" };
const std::string exampleJson(R"({"oid": "a3a5d9", "baseLocation": {"longitude": -78.2338, "latitude": 42.2997}, "mbrRegion": {"pointLow": {"longitude": -78.2338, "latitude": 42.2997}, "pointHigh": {"longitude": -78.2338, "latitude": 42.2997}}})");

int exampleDataLoad() {
    modb::DatabaseResource<modb::Plane> dbResource{dbFileName, DB_BTREE};

    modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, {0.3, 0.3} } };

    const std::string planeOid = plane.oid();
    std::string serialized = dbResource.Serializer_().Serialize(plane);

    dbResource.WriteKeyValuePair(planeOid, serialized, modb::WRITE_APPEND);

    modb::Plane readRecord = dbResource.FindById(planeOid);

    std::cout << "key is " << readRecord.oid() << " \t" << "value is " << readRecord.baseLocation().longitude() << readRecord.baseLocation().latitude() << std::endl;
    return 0;
}

int exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, { 0.3, 0.3 } } };

        std::ostringstream outputStream{};
        boost::archive::binary_oarchive outputArchive{outputStream};

        std::string planeOid = plane.oid();

        std::ostringstream oss{};
        boost::archive::binary_oarchive oa(oss);

        oa << plane;
        std::string serialized{oss.str()};

        Dbt key(const_cast<char*>(planeOid.data()), static_cast<uint32_t>(planeOid.length()));
        Dbt value(const_cast<char*>(serialized.data()), static_cast<uint32_t>(serialized.length()));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << planeOid << "'" << std::endl;
            return 1;
        }

        std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

        std::istringstream iss{newObject};
        boost::archive::binary_iarchive ia{iss};

        modb::Plane readRecord;

        ia >> readRecord;

        std::cout << "key is " << readRecord.oid() << std::endl;
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

void examplePipe() {
    std::string line;
    while (true) {
        std::getline(std::cin, line);

        if (line.empty()) {
            // BusyWaiting();
            continue;
        }

        json data = json::parse(line);

        modb::Plane parsedPlane{data};

        std::cout << parsedPlane.oid() << '\n'
            << parsedPlane.baseLocation().longitude() << '\n'
            << parsedPlane.mbrRegion().pointLow().longitude() << "\n\n";
    }

}

int main(int argc, char** argv) {
    examplePipe();

    std::cout << "quitting";
}
