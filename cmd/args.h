#pragma once

#include <random>
#include <string>

class Args {
    private:
        int argc;
        char **argv;
        bool parsed = false;

        auto help() -> void;
        auto parse_seed(std::string seed_str) -> void;
        auto parse_num_sample(std::string n_str) -> void;
        auto parse_filename(std::string filename) -> void;
        auto parse() -> void;

    public:
        std::size_t n;
        std::string filename;
        std::random_device::result_type seed;

        Args(int argc, char **argv);
};

