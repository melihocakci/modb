#include <modb/database.hpp>

int main() {
    modb::database<int, 2> db{"test"};
    return 0;
}
