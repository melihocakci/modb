#include <iostream>

#include <fstream>

int main(int, char** argv) {

    std::ifstream file{argv[1]};
    
    while (true)
    {
        /* code */
        std::string line;
        
        std::getline(file, line);
        std::cout << line << '\n';
    }
    
}