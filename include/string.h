#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class MyString {
    private:
        std::string const str;
        bool const _is_basic;

        friend struct std::hash<MyString>;

    public:
        MyString() = delete;
        MyString(std::string);
        MyString(MyString const&) = delete;
        MyString(MyString&&) = default;

        auto operator=(MyString const&) -> MyString& = delete;
        auto operator=(MyString&&) -> MyString& = default;

        auto operator<(MyString const&) const noexcept -> bool;

        auto is_basic() const noexcept -> bool;
        auto size() const noexcept -> std::size_t;

        auto disassembly() const noexcept -> std::vector<std::pair<MyString,MyString>>;

        friend auto operator<<(std::ostream& out, MyString const& s) noexcept -> std::ostream&;
};

namespace std {
    template <> struct hash<MyString> {
        typedef MyString argument_type;
        typedef std::size_t result_type;

        auto operator()(argument_type const& arg) const noexcept {
            return std::hash<std::string>{}(arg.str);
        }
    };
}