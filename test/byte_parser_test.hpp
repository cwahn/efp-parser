#ifndef BYTE_PARSER_TEST_HPP_
#define BYTE_PARSER_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "parser.hpp"
// #include "test_common.hpp"

using namespace efp::parser;

TEST_CASE("Tag parser works correctly", "[Tag]")
{
    SECTION("StrinbView starts with the tag")
    {
        auto result = tag("<tag>")("<tag>value");

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 5); // "<tag>"
        }
    }

    SECTION("StrinbView does not start with the tag")
    {
        auto result = tag("<tag>")("value<tag>");
        CHECK_FALSE(result);
    }

    // todo Generic tag
    // struct TokenA
    // {
    // };
    // struct TokenB
    // {
    // };
    // using Token = efp::Enum<TokenA, TokenB>;

    // SECTION("Sequence starts with the tag")
    // {
    //     auto result = tag(efp::Vector<Token>(TokenA{}, TokenA{}))(efp::Vector<Token>(TokenA{}, TokenA{}, TokenB{}));

    //     CHECK(result);
    //     if (result)
    //     {
    //         CHECK(fst(result.value()).size() == 1); // "value"
    //         CHECK(snd(result.value()).size() == 2); // "<tag>"
    //     }
    // }

    // SECTION("Sequence does not start with the tag")
    // {
    //     auto result = tag("<tag>")("value<tag>");
    //     CHECK_FALSE(result);
    // }
}

#endif