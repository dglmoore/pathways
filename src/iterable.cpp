#include <string>
#include <iostream>
#include "../include/pathways.h"
#include "random.h"

namespace pathways {
    class StringDisassembly {
        private:
            std::string root;

        public:
            class const_iterator {
                private:
                    std::string head;
                    std::string tail;
                public:
                    const_iterator() = default;

                    const_iterator(std::string const& str, bool done = false) {
                        if (std::size(str) < 2) {
                            throw std::invalid_argument("string is too short to create an iterator");
                        }
                        if (done) {
                            head = str;
                            tail = "";
                        } else {
                            head = str.substr(0, 1);
                            tail = str.substr(1);
                        }
                    }

                    auto operator++() -> const_iterator& {
                        head += tail[0];
                        tail = tail.substr(1);
                        return *this;
                    }

                    auto operator++(int) -> const_iterator {
                        auto temp = *this;
                           operator++();
                        return temp;
                    }

                    auto operator*() const -> pathways::Components<std::string> {
                        return { head, tail };
                    }

                    auto operator!=(const_iterator const& other) const -> bool {
                        return head != other.head && tail != other.tail;
                    }
            };

            StringDisassembly(std::string const& str): root{str} {
                if (std::size(str) < 2) {
                    throw std::invalid_argument("string is too short to disassemble");
                }
            }

            auto begin() const -> const_iterator {
                return const_iterator(this->root, false);
            }

            auto end() const -> const_iterator {
                return const_iterator(this->root, true);
            }
    };

    template <>
    struct disassembly_type<std::string> {
        using value = StringDisassembly;
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
    auto disassemble<std::string>(std::string const& str) -> StringDisassembly {
        return StringDisassembly(str);
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
