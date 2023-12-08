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

TEST_CASE("hex_digit0 and hex_digit1 parsers work correctly")
{
    SECTION("hex_digit0 with hexadecimal string")
    {
        auto result = hex_digit0("1a2B3cdefGHI");
        CHECK(result);
        CHECK(fst(result.value()) == "GHI");
        CHECK(snd(result.value()) == "1a2B3cdef");
    }

    SECTION("hex_digit0 with non-hexadecimal string")
    {
        auto result = hex_digit0("GHI1a2B3c");
        CHECK(result);
        CHECK(fst(result.value()) == "GHI1a2B3c");
        CHECK(snd(result.value()) == "");
    }

    SECTION("hex_digit1 with hexadecimal string")
    {
        auto result = hex_digit1("1a2B3cdefGHI");
        CHECK(result);
        CHECK(fst(result.value()) == "GHI");
        CHECK(snd(result.value()) == "1a2B3cdef");
    }

    SECTION("hex_digit1 with non-hexadecimal string")
    {
        auto result = hex_digit1("GHI1a2B3c");
        CHECK_FALSE(result);
    }
}

// TEST_CASE("Integer parsers work correctly")
// {
//     SECTION("int with valid number")
//     {
//         auto result = int_parser("1234abc");
//         CHECK(result);
//         CHECK(fst(result.value()) == "abc");
//         CHECK(snd(result.value()) == 1234);
//     }

//     SECTION("uint with valid number")
//     {
//         auto result = uint_parser("4321def");
//         CHECK(result);
//         CHECK(fst(result.value()) == "def");
//         CHECK(snd(result.value()) == 4321u);
//     }

//     SECTION("Invalid number")
//     {
//         auto result = int_parser("abc123");
//         CHECK_FALSE(result);
//     }
// }

TEST_CASE("line_ending parser works correctly", "[line_ending]")
{
    SECTION("String with CRLF line ending")
    {
        auto result = line_ending("\r\ntest");
        CHECK(result);
        CHECK(fst(result.value()) == "test");
        CHECK(snd(result.value()) == "\r\n");
    }

    SECTION("String with LF line ending")
    {
        auto result = line_ending("\ntest");
        CHECK(result);
        CHECK(fst(result.value()) == "test");
        CHECK(snd(result.value()) == "\n");
    }

    SECTION("String without line ending")
    {
        auto result = line_ending("test");
        CHECK_FALSE(result);
    }
}

TEST_CASE("multispace0 and multispace1 parsers work correctly")
{
    SECTION("multispace0 with whitespace string")
    {
        auto result = multispace0("   abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "   ");
    }

    SECTION("multispace0 with non-whitespace string")
    {
        auto result = multispace0("abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "");
    }

    SECTION("multispace1 with whitespace string")
    {
        auto result = multispace1("   abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "   ");
    }

    SECTION("multispace1 with non-whitespace string")
    {
        auto result = multispace1("abc");
        CHECK_FALSE(result);
    }
}

TEST_CASE("newline parser works correctly", "[newline]")
{
    SECTION("String starting with newline")
    {
        auto result = newline("\ntest");
        CHECK(result);
        CHECK(fst(result.value()) == "test");
        CHECK(snd(result.value()) == '\n');
    }

    SECTION("String not starting with newline")
    {
        auto result = newline("test\n");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = newline("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("NoneOfParser works correctly", "[none_of]")
{
    auto parser = none_of("aeiou");

    SECTION("String starting with a non-matching character")
    {
        auto result = parser("test");
        CHECK(result);
        CHECK(fst(result.value()) == "est");
        CHECK(snd(result.value()) == 't');
    }

    SECTION("String starting with a matching character")
    {
        auto result = parser("apple");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = parser("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("not_line_ending parser works correctly", "[not_line_ending]")
{
    SECTION("String without line ending")
    {
        auto result = not_line_ending("test string");
        CHECK(result);
        CHECK(fst(result.value()) == "");
        CHECK(snd(result.value()) == "test string");
    }

    SECTION("String with line ending")
    {
        auto result = not_line_ending("test\nstring");
        CHECK(result);
        CHECK(fst(result.value()) == "\nstring");
        CHECK(snd(result.value()) == "test");
    }

    SECTION("String that is a line ending")
    {
        auto result = not_line_ending("\n");
        CHECK_FALSE(result);
    }
}

TEST_CASE("oct_digit0 and oct_digit1 parsers work correctly")
{
    SECTION("oct_digit0 with octal string")
    {
        auto result = oct_digit0("12345670abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "12345670");
    }

    SECTION("oct_digit0 with non-octal string")
    {
        auto result = oct_digit0("9abc");
        CHECK(result);
        CHECK(fst(result.value()) == "9abc");
        CHECK(snd(result.value()) == "");
    }

    SECTION("oct_digit1 with octal string")
    {
        auto result = oct_digit1("12345670abc");
        CHECK(result);
        CHECK(fst(result.value()) == "abc");
        CHECK(snd(result.value()) == "12345670");
    }

    SECTION("oct_digit1 with non-octal string")
    {
        auto result = oct_digit1("9abc");
        CHECK_FALSE(result);
    }
}

TEST_CASE("OneOfParser works correctly", "[one_of]")
{
    OneOfParser parser("aeiou");

    SECTION("String starting with a matching character")
    {
        auto result = parser("apple");
        CHECK(result);
        CHECK(fst(result.value()) == "pple");
        CHECK(snd(result.value()) == 'a');
    }

    SECTION("String starting with a non-matching character")
    {
        auto result = parser("test");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = parser("");
        CHECK_FALSE(result);
    }
}

TEST_CASE("SatisfyParser works correctly", "[satisfy]")
{
    auto vowel_parser = satisfy([](char c)
                                { return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'; });

    SECTION("String starting with a vowel")
    {
        auto result = vowel_parser("apple");
        CHECK(result);
        // ... assertions ...
    }

    SECTION("String starting with a vowel")
    {
        auto result = vowel_parser("apple");
        CHECK(result);
        CHECK(fst(result.value()) == "pple");
        CHECK(snd(result.value()) == 'a');
    }

    SECTION("String starting with a non-vowel")
    {
        auto result = vowel_parser("test");
        CHECK_FALSE(result);
    }

    SECTION("Empty string")
    {
        auto result = vowel_parser("");
        CHECK_FALSE(result);
    }
}

#endif