#pragma once

#include "pathways.h"
#include <stdexcept>
#include <vector>

namespace pathways {
    template <>
    struct disassembly_type<int> {
        using value = std::vector<Components<int>>;
    };

    template <>
    auto is_basic<int>(int const& x) -> bool {
        if (x < 1) {
            throw std::invalid_argument("integers less than 1 are not in the space");
        }
        return x == 1;
    }

    template <>
    auto is_below<int>(int const& x, int const& y) -> bool {
        if (x < 1 || y < 1) {
            throw std::invalid_argument("integers less than 1 are not in the space");
        }
        return x <= y;
    }

    template <>
    auto disassemble<int>(int const& n) -> std::vector<Components<int>> {
        if (n < 1) {
            throw std::invalid_argument("integers less than 1 are not in the space");
        }
        auto parts = std::vector<Components<int>>{};
        if (n != 1) {
            for (int left = 1; left <= (n - (n & 1)) / 2; ++left) {
                parts.emplace_back(left, n - left);
            }
        }
        return parts;
    }

    template class Context<int>;
}

