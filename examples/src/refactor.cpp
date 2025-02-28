#include <modb/database.hpp>

int main() {
    const modb::database<int, 2> db{"test"};
    return 0;
}
