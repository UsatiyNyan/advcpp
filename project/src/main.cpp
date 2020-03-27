//
// Created by kira on 21.02.2020.
//

#include <iostream>
#include "process.h"

int main() {
    try {
        std::cout << std::boolalpha;

        process::Process proc("./example");

        std::string command("ORAORA\n");
        proc.write_exact(command.c_str(), command.length());

        char output[100] = {};
        proc.read_exact(output, command.length());
        std::cout << output << std::endl;
        std::cout << proc.is_readable() << std::endl;

        proc.close_stdin();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
