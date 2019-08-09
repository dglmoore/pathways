#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include "mystring.h"

MyString::MyString(std::string str): str{str}, _is_basic(str.size() == 1) {
    if (str.empty()) {
        throw std::invalid_argument("string is empty");
    }
}

auto MyString::is_below(MyString const& other) const noexcept -> bool {
    return other.str.find(this->str) != std::string::npos;
}

auto MyString::is_basic() const noexcept -> bool {
    return this->_is_basic;
}

auto MyString::size() const noexcept -> std::size_t {
    return std::size(this->str);
}

auto MyString::disassemble() const noexcept -> disassembly_type {
    auto parts = disassembly_type{};
    for (size_t i = 1, len = std::size(*this); i < len; ++i) {
        parts.emplace_back(this->str.substr(0, i), this->str.substr(i));
    }
    return parts;
}

auto operator<<(std::ostream& out, MyString const& s) noexcept -> std::ostream& {
    return out << std::quoted(s.str);
}
