//
// Created by kira on 21.02.2020.
//

#include "global_logger.h"
#include "process.h"

int main(int argc, char *argv[]) {
    try {
        log::Logger::get_instance().set_global_logger(log::create_file_logger("log.txt"));
        process::Process proc("./echo");

        std::string command("MUDAMUDAMUDAMUDAMUDAMUDA\n");
        proc.write_exact(command.c_str(), command.length());

        char output[100] = {};
        proc.read_exact(output, command.length());
        log::debug(output);
        log::info(output);
        log::warn(output);
        log::error(output);
        proc.close_stdin();
    } catch (std::exception &e) {
        log::error(e.what());
    }

    return 0;
}
