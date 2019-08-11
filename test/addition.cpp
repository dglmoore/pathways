#include "catch2/catch.hpp"
#include <algorithm>
#include <numeric>
#include <pathways/addition.h>

#include <iostream>

TEST_CASE("ints satisfy the pathways interface", "[addition]") {
    using namespace pathways;

    SECTION("is_basic throws for invalid int") {
        REQUIRE_THROWS_AS(is_basic(0), std::invalid_argument);
        REQUIRE_THROWS_AS(is_basic(-1), std::invalid_argument);
    }

    SECTION("is_basic") {
        REQUIRE(is_basic(1));
        REQUIRE(!is_basic(10));
    }

    SECTION("is_below throws for invalid int") {
        REQUIRE_THROWS_AS(is_below(0, 3), std::invalid_argument);
        REQUIRE_THROWS_AS(is_below(3, 0), std::invalid_argument);
        REQUIRE_THROWS_AS(is_below(-1, 3), std::invalid_argument);
        REQUIRE_THROWS_AS(is_below(3, -1), std::invalid_argument);
    }

    SECTION("is_below") {
        REQUIRE(is_below(1, 1));
        REQUIRE(is_below(1, 2));
        REQUIRE(is_below(2, 3));

        REQUIRE(!is_below(2, 1));
        REQUIRE(!is_below(3, 2));
    }

    SECTION("disassemble throws for invalid int") {
        REQUIRE_THROWS_AS(disassemble(-1), std::invalid_argument);
        REQUIRE_THROWS_AS(disassemble(0), std::invalid_argument);
    }

    SECTION("disassemble returns a collection with the correct length") {
        REQUIRE(disassemble(1).empty());
        for (int n = 2; n <= 10; ++n) {
            REQUIRE(std::size(disassemble(n)) == (n - (n & 1)) / 2);
        }
    }

    SECTION("can dissassemble an int") {
        using container = typename disassembly_type<int>::value;
        container const expected = { {1, 6}, {2, 5}, {3, 4} };
        container const got = disassemble(7);

        REQUIRE(std::size(got) == std::size(expected));
        REQUIRE(std::equal(std::begin(got), std::end(got), std::begin(expected)));
    }
}

TEST_CASE("can estimate the assembly index for an int", "[addition]") {
    using namespace pathways;

    SECTION("assembly index for the first 12 ints") {
        // While we know what the correct answer is, we also know that the
        // current implementation is only an approximate solution. We keep this
        // correct expectation here for subsequent improvements to the
        // algorithm.
        //
        // std::vector<int> expected = { 0, 1, 2, 2, 3, 3, 4, 3, 4, 4, 5, 4 };
        std::vector<int> expected = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4 };

        Context<int> ctx;
        auto const assembly_index = [&ctx](int x){ return ctx.assembly_index(x); };
        std::vector<int> got(std::size(expected));
        std::iota(std::begin(got), std::end(got), 1);
        std::transform(std::begin(got), std::end(got), std::begin(got), assembly_index);

        REQUIRE(std::equal(std::begin(got), std::end(got), std::begin(expected)));
    }
}
