#include <iostream>
#include <unistd.h>
#include <thread>
#include <wait.h>
#include <chrono>

#include "shmem.h"

using namespace std::chrono_literals;

int main(int argc, char** argv) {
    shmem::map<shmem::string, shmem::string> my_map(4096);
    int child = fork();
    if (child) {
        my_map.lock();
        my_map["Jotaro"] = "ORAORAORA";
    } else {
        my_map.lock();
        my_map["DIO"] = "MUDAMUDAMUDA";
        return 0;
    }
    std::this_thread::sleep_for(1s);
    for (const auto & [key, value]: my_map) {
        std::cout << key << ' ' << value << std::endl;
    }
    waitpid(child, nullptr, 0);
    return 0;
}