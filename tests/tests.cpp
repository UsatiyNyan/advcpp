//
// Created by kira on 21.02.2020.
//
#include <gtest/gtest.h>
#include "process.h"


class TestProcess : public ::testing::Test {
 protected:
    void SetUp() {
        proc = new process::Process("../project/echo");
    }
    void TearDown() {
        delete proc;
    }
    process::Process *proc;
};

TEST_F(TestProcess, many_words) {
    for (int i = 0; i < 10000; ++i) {
        std::string command = std::to_string(i) + '\n';
        proc->write(command.c_str(), command.length());
    }

    while (true) {
        char output[256] = {};
        proc->read(output, sizeof(output));
        std::cout << output << std::endl;
        if (!proc->is_readable()) {
            break;
        }
    }
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}