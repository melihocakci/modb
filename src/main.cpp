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

int exampleDataLoad() {
    modb::DatabaseResource<modb::Plane> dbResource{dbFileName, DB_BTREE};
    
    modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, {0.3, 0.3} };

    const std::string planeOid = plane.Oid();
    std::string serialized = dbResource.Serializer_().Serialize(plane);

    dbResource.WriteKeyValuePair(planeOid, serialized);

    modb::Plane readRecord = dbResource.FindById(planeOid);

    std::cout << "key is " << readRecord.Oid() << " \t" << "value is " << readRecord.MbrRectangle().m_height <<  readRecord.MbrRectangle().m_width << std::endl;
   return 0;
}

int exampleLoad() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName.c_str(), NULL, DB_BTREE, DB_CREATE, 0);

        modb::Plane plane{ "a3a5d9", { 1.2, 1.3 }, { 2.2, 1.2 }, {1.1,1.1} };
        std::string planeOid = plane.Oid();

        std::ostringstream oss{};
        boost::archive::binary_oarchive oa(oss);

        oa << plane;
        std::string serialized{oss.str()};

        Dbt key(const_cast<char*>(planeOid.c_str()), static_cast<uint32_t>(planeOid.length() + 1));
        Dbt value(const_cast<char*>(serialized.c_str()), static_cast<uint32_t>(serialized.length() + 1));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << planeOid << "'" << std::endl;
            return 1;
        }

        uint32_t tmp = retVal.get_size();

        std::string newObject{reinterpret_cast<char*>(retVal.get_data()), retVal.get_size()};

        std::istringstream iss{newObject};
        boost::archive::binary_iarchive ia{iss};

        modb::Plane readRecord;

        ia >> readRecord;

        std::cout << "key is " << readRecord.Oid() << std::endl;
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
    exampleDataLoad();


}
