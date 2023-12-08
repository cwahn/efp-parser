#ifndef TERMINAL_PARSER_TEST_HPP_
#define TERMINAL_PARSER_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "parser.hpp"
// #include "test_common.hpp"

using namespace efp::parser;

TEST_CASE("start_with function works correctly", "[start_with]")
{
    SECTION("Sequence starts with the given prefix")
    {
        CHECK(start_with("hello", "he"));
    }

    SECTION("Sequence does not start with the given prefix")
    {
        CHECK_FALSE(start_with("hello", "lo"));
    }
}

TEST_CASE("alpha0 parser works correctly", "[alpha0]")
{
    SECTION("Sequence with only alphabetic characters")
    {
        auto result = alpha0("abcde");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "abcde");
    }

    SECTION("Empty sequence")
    {
        auto result = alpha0("");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "");
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha0("123abc");
        CHECK(result);
        CHECK(fst(result.value()) == "123abc");
        CHECK(snd(result.value()) == "");
    }
}

TEST_CASE("alpha1 parser works correctly", "[alpha1]")
{
    SECTION("Sequence with only alphabetic characters")
    {
        auto result = alpha1("abcde");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "abcde");
    }

    SECTION("Empty sequence")
    {
        auto result = alpha1("");
        CHECK_FALSE(result);
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha1("123abc");
        CHECK_FALSE(result);
    }
}

TEST_CASE("alphanumeric0 parser works correctly", "[alphanumeric0]")
{
    SECTION("Alphanumeric sequence")
    {
        auto result = alphanumeric0("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "abc123");
    }

    SECTION("Non-alphanumeric characters")
    {
        auto result = alphanumeric0("!@#");
        CHECK(result);
        CHECK(fst(result.value()) == "!@#");
        CHECK(snd(result.value()) == "");
    }

    SECTION("Empty string")
    {
        auto result = alphanumeric0("");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "");
    }
}

TEST_CASE("alphanumeric1 parser works correctly", "[alphanumeric1]")
{
    SECTION("Alphanumeric sequence")
    {
        auto result = alphanumeric1("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "abc123");
    }

    SECTION("Non-alphanumeric characters")
    {
        auto result = alphanumeric1("!@#");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = alphanumeric1("");
        CHECK_FALSE(result);
    }
}

#endif