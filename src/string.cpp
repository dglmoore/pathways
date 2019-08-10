#include <string>
#include <iostream>
#include "../include/pathways.h"
#include "random.h"

namespace pathways {
	template <>
	struct disassembly_type<std::string> {
		using value = std::vector<Components<std::string>>;
	};

    template <>
    auto is_basic<std::string>(std::string const& str) -> bool {
        return std::size(str) == 1;
    }
    
    template <>
    auto is_below<std::string>(std::string const& x, std::string const& y) -> bool {
        return y.find(x) != std::string::npos;
    }
    
    template <>
    auto disassemble<std::string>(std::string const& str) -> std::vector<Components<std::string>> {
    	if (str.empty()) {
    		throw std::invalid_argument("string is empty");
    	}
        auto parts = std::vector<Components<std::string>>{};
        for (size_t i = 1, len = std::size(str); i < len; ++i) {
            parts.emplace_back(str.substr(0, i), str.substr(i));
        }
        return parts;
    }
}

auto main() -> int {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    auto const str = random_string(200, gen);
    pathways::Context<std::string> ctx;
    std::cout << "c ~ " << ctx.assembly_index(str) << std::endl;
    std::cout << "Cache size: " << ctx.cache_size() << std::endl;
}
