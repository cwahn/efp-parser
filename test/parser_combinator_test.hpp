#ifndef PARSER_COMBINATOR_TEST_HPP_
#define PARSER_COMBINATOR_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "parser.hpp"
// #include "test_common.hpp"

using namespace efp::parser;

TEST_CASE("AltParser parser works correctly", "[alt]")
{
    SECTION("Alternative with character matches")
    {
        auto result = alt(tag("!"), tag("<tag>"))("!value");
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()) == "value");
            CHECK(snd(result.value()) == "!");
        }
    }

    SECTION("Alternative with tag matches")
    {
        auto result = alt(tag("!"), tag("<tag>"))("<tag>value");
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()) == "value");
            CHECK(snd(result.value()) == "<tag>");
        }
    }

    SECTION("No alternatives match")
    {
        auto result = alt(tag("!"), tag("<tag>"))("value");
        CHECK_FALSE(result);
    }
}

TEST_CASE("alt parser with nested alternatives works correctly", "[alt]")
{
    SECTION("Nested alternatives with multiple matching parsers")
    {
        auto nested_alt = alt(tag("a"), alt(tag("b"), tag("tag")), digit1);

        // Test case where the first parser matches
        {
            auto result = nested_alt("apple");
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()) == "pple");
                CHECK(snd(result.value()) == "a");
            }
        }

        // Test case where the nested alternative matches
        {
            auto result = nested_alt("tagvalue");
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()) == "value");
                CHECK(snd(result.value()) == "tag");
            }
        }

        // Test case where the last parser matches
        {
            auto result = nested_alt("1234");
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()).empty()); // All input consumed
                CHECK(snd(result.value()) == "1234");
            }
        }

        // Test case where no parsers match
        {
            auto result = nested_alt("!none");
            CHECK_FALSE(result);
        }
    }
}

TEST_CASE("tuple parser combinator works correctly", "[tpl]")
{
    auto tpl_parser = tpl(tag("a"), tag("b"), digit1);

    SECTION("All parsers match in sequence")
    {
        auto result = tpl_parser("ab12345");
        CHECK(result);
        if (result)
        {
            auto res = snd(result.value());
            CHECK(efp::p<0>(res) == "a");
            CHECK(efp::p<1>(res) == "b");
            CHECK(efp::p<2>(res) == "12345");
        }
    }

    SECTION("First parser fails")
    {
        auto result = tpl_parser("zb12345");
        CHECK_FALSE(result);
    }

    SECTION("Middle parser fails")
    {
        auto result = tpl_parser("a12345");
        CHECK_FALSE(result);
    }

    SECTION("Last parser fails")
    {
        auto result = tpl_parser("ababc");
        CHECK_FALSE(result);
    }

    SECTION("Empty input")
    {
        auto result = tpl_parser("");
        CHECK_FALSE(result);
    }
}

#endif