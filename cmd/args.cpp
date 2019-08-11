#include "args.h"

#include <cmath>
#include <iostream>

auto Args::help() -> void {
    std::cerr << "usage: " << argv[0] << " [OPTIONS] <NUM SAMPLES> <FILENAME>\n"
              << "\n"
              << "Args:\n"
              << "\t<NUM SAMPLES>    number of samples for each datapoint\n"
              << "\t<FILENAME>       output filename\n"
              << "\n"
              << "OPTIONS:\n"
              << "\t-s <seed>        the random seed\n"
              << std::endl;
    std::exit(1);
}

auto Args::parse_seed(std::string seed_str) -> void {
    auto s = std::stof(seed_str);
    if (std::floor(s) != s) {
        std::cerr << "error: seed must be an integer\n" << std::endl;
        help();
    } else if (s < 0) {
        std::cerr << "error: seed must be at least 0\n" << std::endl;
        help();
    }
    this->seed = s;
}

auto Args::parse_num_sample(std::string n_str) -> void {
    auto n = std::stof(n_str);
    if (std::floor(n) != n) {
        std::cerr << "error: number of samples must be an integer\n" << std::endl;
        help();
    } else if (n < 1) {
        std::cerr << "error: number of samples must be at least 1\n" << std::endl;
        help();
    }
    this->n = n;
}

auto Args::parse_filename(std::string filename) -> void {
    this->filename = filename;
}

auto Args::parse() -> void{
    std::size_t p = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-s") {
            parse_seed(argv[++i]);
        } else if (p == 0) {
            parse_num_sample(arg);
            ++p;
        } else if (p == 1) {
            parse_filename(arg);
            ++p;
        } else if (arg[0] == '-') {
            std::cerr << "error: unrecognized flag\n" << std::endl;
            help();
        } else {
            std::cerr << "error: unrecognized argument\n" << std::endl;
            help();
        }
    }
    if (p != 2) {
        help();
    }
    parsed = true;
}

Args::Args(int argc, char **argv): argc{argc}, argv{argv} {
    this->parse();
}
