#include <modb/database.hpp>

struct container {
    std::string str;
    int num[2];

    template <class Archive>
    void serialize(Archive& ar, unsigned int) {
        ar& str;
        ar& num;
    }
};

int main() {
    constexpr int dimensions = 2;
    const modb::database<container, dimensions> db{ "test" };

    db.put_object(1, { 0.0, 0.0 }, { "miku", 01, 16 });
    db.put_object(2, { 0.5, 0.5 }, { "teto", 0401, 31 });

    const auto res = db.intersection_query({ { -1.0, -1.0 }, { 1.0, 1.0 } });
    for (const auto& obj : res) {
        std::cout << "id: " << obj.id << '\n';

        std::cout << "location:\n";
        for (double i : obj.location.arr) {
            std::cout << i << '\n';
        }

        std::cout << "region.min:" << '\n';
        for (double i : obj.region.min.arr) {
            std::cout << i << '\n';
        }

        std::cout << "region.max:" << '\n';
        for (double i : obj.region.max.arr) {
            std::cout << i << '\n';
        }

        std::cout << "data.str: " << obj.data.str << '\n';

        std::cout << "data.num: " << '\n';
        for (int i : obj.data.num) {
            std::cout << i << '\n';
        }

        std::cout << "----------------\n";
    }
}
