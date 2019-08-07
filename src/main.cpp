#include <random>
#include "pathways.h"
#include "string.h"

template <typename Generator>
auto random_string(std::size_t n, Generator &gen, double p = 0.5) -> MyString {
    std::bernoulli_distribution d(p);

    auto str = std::string(n, '\0');
    std::generate_n(std::begin(str), n, [&gen, &d]() { return d(gen) ? '1' : '0'; });

    return str;
}

auto main() -> int {
    std::random_device rd;
    std::mt19937 gen(rd());

    Context<MyString> ctx;
    auto const str = random_string(200, gen);
    std::cout << "c ~ " << ctx.assembly_index(str) << std::endl;
    std::cout << "Cache size: " << ctx.cache_size() << std::endl;
}
