#include <iostream>

#include <fstream>

int main(int arc, char ** argv) {
    std::ifstream file {argv[1]};

    std::cout << argv[1] << "opened" << std::endl;
    return 0;
}