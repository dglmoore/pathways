#include <iostream>
#include "../include/pathways.h"
#include "random.h"
#include "string.h"

auto main() -> int {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto const str = random_string(200, gen);
    pathways::Context<std::string> ctx;
    std::cout << "c ~ " << ctx.assembly_index(str) << std::endl;
    std::cout << "Cache size: " << ctx.cache_size() << std::endl;
}
