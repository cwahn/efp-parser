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

TEST_CASE("anychar parser works correctly", "[anychar]")
{
    SECTION("Non-empty string")
    {
        auto result = anychar("test");
        CHECK(result);
        CHECK(fst(result.value()) == "est");
        CHECK(snd(result.value()) == 't');
    }

    SECTION("Empty string")
    {
        auto result = anychar("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("char parser works correctly", "[char]")
{
    auto parse_a = ch('a');

    SECTION("String starting with target character")
    {
        auto result = parse_a("apple");
        CHECK(result);
        CHECK(fst(result.value()) == "pple");
        CHECK(snd(result.value()) == 'a');
    }

    SECTION("String not starting with target character")
    {
        auto result = parse_a("banana");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = parse_a("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("crlf parser works correctly", "[crlf]")
{
    SECTION("String starting with CRLF")
    {
        auto result = crlf("\r\ntest");
        CHECK(result);
        CHECK(fst(result.value()) == "test");
        CHECK(snd(result.value()) == "\r\n");
    }

    SECTION("String not starting with CRLF")
    {
        auto result = crlf("test\r\n");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = crlf("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("digit0 and digit1 parsers work correctly")
{
    SECTION("digit0 with numeric string")
    {
        auto result = digit0("1234abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "1234");
    }

    SECTION("digit0 with non-numeric string")
    {
        auto result = digit0("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "abc123");
        CHECK(snd(result.value()) == "");
    }

    SECTION("digit1 with numeric string")
    {
        auto result = digit1("1234abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "1234");
    }

    SECTION("digit1 with non-numeric string")
    {
        auto result = digit1("abc123");
        CHECK_FALSE(result);
    }
}

#endif