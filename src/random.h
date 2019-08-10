#pragma once

#include <algorithm>
#include <random>

template <typename Generator>
auto random_string(std::size_t n, Generator &gen, double p = 0.5) -> std::string {
    std::bernoulli_distribution d(p);

    auto str = std::string(n, '\0');
    std::generate_n(std::begin(str), n, [&gen, &d]() { return d(gen) ? '1' : '0'; });

    return str;
}