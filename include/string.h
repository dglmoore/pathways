#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class MyString {
    private:
        std::string const str;
        bool const _is_basic;

        // This goes hand-in-hand with the `std::hash` specialization below. It
        // is necessary in order for that hasher to access the `str` member of the
        // `MyString` class.
        friend struct std::hash<MyString>;

    public:
        MyString() = delete;

        // Construct a `MyString` instance from an *non-empty* `std::string`.
        MyString(std::string);
        MyString(MyString const&) = delete;
        MyString(MyString&&) = default;

        auto operator=(MyString const&) -> MyString& = delete;
        auto operator=(MyString&&) -> MyString& = default;

        // REQUIRED: You have to be able to compare objects — less than or equal
        // (hard problem!)
        auto operator<=(MyString const&) const noexcept -> bool;

        // REQUIRED: You have to be able to decide if an object is "basic" (easy
        // problem)
        auto is_basic() const noexcept -> bool;

        // This just gives you the size of the string (in characters).
        auto size() const noexcept -> std::size_t;

        // REQUIRED: You have to be able to break your object up into an iterable
        // "container" of pairs (simple problem)
        auto disassembly() const noexcept -> std::vector<std::pair<MyString,MyString>>;

        // This is just to make the strings printable to the screen.
        friend auto operator<<(std::ostream& out, MyString const& s) noexcept -> std::ostream&;
};

namespace std {
    // REQUIRED: You need to be able to generate a hash (of type `std::size`) for
    // your objects.
    template <> struct hash<MyString> {
        typedef MyString argument_type;
        typedef std::size_t result_type;

        auto operator()(argument_type const& arg) const noexcept {
            return std::hash<std::string>{}(arg.str);
        }
    };
}

