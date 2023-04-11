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

const std::string dbFileName{ "Object.db" };
const std::string exampleJson(R"({"id": "a3a5d9", "baseLocation": {"longitude": -78.2338, "latitude": 42.2997}, "mbrRegion": {"pointLow": {"longitude": -78.2338, "latitude": 42.2997}, "pointHigh": {"longitude": -78.2338, "latitude": 42.2997}}})");

int exampleDataLoad() {
    modb::DatabaseResource<modb::Object> dbResource{dbFileName, DB_BTREE};

    modb::Object Object{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, {0.3, 0.3} } };

    const std::string ObjectOid = Object.id();
    std::string serialized = dbResource.Serializer_().Serialize(Object);

    dbResource.WriteKeyValuePair(ObjectOid, serialized, modb::WRITE_APPEND);

    modb::Object readRecord = dbResource.FindById(ObjectOid);

    std::cout << "key is " << readRecord.id() << " \t" << "value is " << readRecord.baseLocation().longitude() << readRecord.baseLocation().latitude() << std::endl;
    return 0;
}

int exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        modb::Object Object{ "a3a5d9", { 1.2, 1.3 }, { { 2.2, 1.2 }, { 0.3, 0.3 } } };

        std::ostringstream outputStream{};
        boost::archive::binary_oarchive outputArchive{outputStream};

        std::string ObjectOid = Object.id();

        std::ostringstream oss{};
        boost::archive::binary_oarchive oa(oss);

        oa << Object;
        std::string serialized{oss.str()};

        Dbt key(const_cast<char*>(ObjectOid.data()), static_cast<uint32_t>(ObjectOid.length()));
        Dbt value(const_cast<char*>(serialized.data()), static_cast<uint32_t>(serialized.length()));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << ObjectOid << "'" << std::endl;
            return 1;
        }

        std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

        std::istringstream iss{newObject};
        boost::archive::binary_iarchive ia{iss};

        modb::Object readRecord;

        ia >> readRecord;

        std::cout << "key is " << readRecord.id() << std::endl;
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

        modb::Object parsedObject{data};

        std::cout << parsedObject.id() << '\n'
            << parsedObject.baseLocation().longitude() << '\n'
            << parsedObject.mbrRegion().pointLow().longitude() << "\n\n";
    }

}

int main(int argc, char** argv) {
    examplePipe();

    std::cout << "quitting";
}
