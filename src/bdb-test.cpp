#include <db_cxx.h>

#include <iostream>
#include <string>

const char* dbFileName{ "test.db" };

class Record {
public:
    Record(int key, int value): m_key{ key }, m_value{ value } {}

    int getKey() { return m_key; }

    int getValue() { return m_value; }

private:
    int m_key;
    int m_value;
};

int main() {
    try
    {
        Db myDb{ NULL, 0 };
        myDb.set_error_stream(&std::cerr);
        myDb.open(NULL, dbFileName, NULL, DB_BTREE, DB_CREATE, 0);

        Record myRecord{ 1, 111 };
        auto myKey = myRecord.getKey();

        Dbt key(reinterpret_cast<void*>(&myKey), static_cast<uint32_t>(sizeof(myKey)));
        Dbt value(reinterpret_cast<void*>(&myRecord), static_cast<uint32_t>(sizeof(myRecord)));

        myDb.put(NULL, &key, &value, 0);

        Dbc* cursorp;
        myDb.cursor(NULL, &cursorp, 0);

        Dbt retVal;
        int ret = cursorp->get(&key, &retVal, DB_SET);

        if (ret) {
            std::cerr << "No records found for '" << myKey << "'" << std::endl;
            return 1;
        }

        Record* readRecord = reinterpret_cast<Record*>(retVal.get_data());

        std::cout << "key is " << readRecord->getKey() << std::endl;
        std::cout << "value is " << readRecord->getValue() << std::endl;
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

}