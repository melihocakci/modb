#include <fstream>
#include <iostream>

int main(int, char ** argv) {
    std::ifstream file{argv[1]};
    std::string line;
    std::getline(file, line);
    std::cout << line << '\n';
}