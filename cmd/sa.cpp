#include "random.h"
#include <iostream>
#include <pathways/string.h>
#include <chrono>
#include <sstream>

using namespace std::chrono;

auto usage(char *cmd) -> void {
    std::stringstream ss;
    ss << "usage: " << cmd << " [--no-cache] <string>";
    throw ss.str();
}

auto args(int argc, char **argv) -> std::tuple<std::string, bool> {
    if (argc == 1) {
	usage(argv[0]);
    }
    auto str = std::string{};
    auto cache = true;
    for (auto i = 1; i < argc; ++i) {
        auto arg = std::string(argv[i]);
        if (arg == "--no-cache") {
            cache = false;
        } else if (str == "") {
            str = arg;
        } else {
            usage(argv[0]);
        }
    }
    return { str, cache };
}

auto main(int argc, char **argv) -> int {
    auto str = std::string{};
    auto cache = false;
    try {
        std::tie(str, cache) = args(argc, argv);
    } catch (std::string &s) {
        std::cerr << s << std::endl;
        return 1;
    }

    pathways::Context<std::string> ctx;

    auto start = high_resolution_clock::now();
    auto const c = ctx.assembly_index(str, cache);
    auto stop = high_resolution_clock::now();
    std::cout << c << std::endl;
    duration<double> elapsed = stop - start;
    std::cout << elapsed.count() << std::endl;
}
