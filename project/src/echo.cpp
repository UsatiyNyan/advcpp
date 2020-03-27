//
// Created by kira on 07.03.2020.
//

#include <iostream>


int main() {
    std::string buf;
    while (!std::cin.eof()) {
        std::cin >> buf;
        std::cout << buf << std::endl;
    }
    return 0;
}
