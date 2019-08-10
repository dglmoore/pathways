#include <iostream>
#include "../include/pathways.h"
#include "random.h"

class MyString {
    private:
        std::string const str;

        friend struct std::hash<MyString>;
    public:
        using disassembly_type = std::vector<pathways::Components<MyString>>;

        MyString() = delete;

        MyString(std::string str): str{std::move(str)} {
        	if (this->str.empty()) {
        		throw std::invalid_argument("string is empty");
        	}
        }

        auto is_basic() const -> bool {
        	return std::size(this->str) == 1;
        }
        
        auto is_below(MyString const& other) const -> bool {
        	return other.str.find(this->str) != std::string::npos;
        }

        auto disassemble() const -> disassembly_type {
            auto parts = disassembly_type{};
            for (size_t i = 1, len = std::size(str); i < len; ++i) {
                parts.emplace_back(str.substr(0, i), str.substr(i));
            }
            return parts;
        }
};

namespace std {
    template <> struct hash<MyString> {
        auto operator()(MyString const& arg) const noexcept -> std::size_t {
            return std::hash<std::string>{}(arg.str);
        }
    };
}

auto main() -> int {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    auto const str = random_string(200, gen);
    pathways::Context<MyString> ctx;
    std::cout << "c ~ " << ctx.assembly_index(str) << std::endl;
    std::cout << "Cache size: " << ctx.cache_size() << std::endl;
}
