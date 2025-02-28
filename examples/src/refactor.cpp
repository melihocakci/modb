#include <modb/database.hpp>

int main() {
    const modb::database<int, 2> db{"test"};

    db.put_object(1, {0.0, 0.0}, 42);
    const auto [ret, obj] = db.get_object(1);

    std::cout << ret;
}
