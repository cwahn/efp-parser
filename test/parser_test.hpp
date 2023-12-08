#ifndef PARSER_TEST_HPP_
#define PARSER_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "parser.hpp"
// #include "test_common.hpp"

using namespace efp;
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

TEST_CASE("Ch parser works correctly", "[Ch]")
{

    SECTION("Sequence starts with the character")
    {
        auto result = ch('a')("apple");

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()) == "pple");
            CHECK(snd(result.value()) == "a");
        }
    }

    SECTION("Sequence does not start with the character")
    {
        auto result = ch('a')("banana");

        CHECK_FALSE(result);
    }
}

// EFP_PARSER_TAG(ParseTag, "<tag>")

TEST_CASE("Tag parser works correctly", "[Tag]")
{
    SECTION("Sequence starts with the tag")
    {
        auto result = tag("<tag>")(StringView{"<tag>value", 10});
        // auto result = ParseTag{}(StringView{"<tag>value", 10});

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 5); // "<tag>"
        }
    }

    SECTION("Sequence does not start with the tag")
    {
        auto result = tag("<tag>")(StringView{"value<tag>", 10});
        // auto result = ParseTag{}(StringView{"value<tag>", 10});
        CHECK_FALSE(result);
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

TEST_CASE("numeric0 parser works correctly", "[numeric0]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric0("12345");
        CHECK(result);
        CHECK(fst(result.value()) == "");      // Direct string comparison
        CHECK(snd(result.value()) == "12345"); // Direct string comparison
    }

    SECTION("Empty sequence")
    {
        auto result = numeric0("");
        CHECK(result);
        CHECK(fst(result.value()) == ""); // Direct string comparison
        CHECK(snd(result.value()) == ""); // Direct string comparison
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric0("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "abc123"); // Direct string comparison
        CHECK(snd(result.value()) == "");       // Direct string comparison
    }
}

TEST_CASE("numeric1 parser works correctly", "[numeric1]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric1("12345");
        CHECK(result);
        CHECK(fst(result.value()) == "");      // Direct string comparison
        CHECK(snd(result.value()) == "12345"); // Direct string comparison
    }

    SECTION("Empty sequence")
    {
        auto result = numeric1("");
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric1("abc123");
        CHECK_FALSE(result); // Should fail as the first character is not numeric
    }
}

TEST_CASE("alphanum0 parser works correctly", "[alphanum0]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum0("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "");       // Direct string comparison
        CHECK(snd(result.value()) == "abc123"); // Direct string comparison
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum0("");
        CHECK(result);
        CHECK(fst(result.value()) == ""); // Direct string comparison
        CHECK(snd(result.value()) == ""); // Direct string comparison
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum0("!@#abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "!@#abc123"); // Direct string comparison
        CHECK(snd(result.value()) == "");          // Direct string comparison
    }
}

TEST_CASE("alphanum1 parser works correctly", "[alphanum1]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum1("abc123");
        CHECK(result);
        CHECK(fst(result.value()) == "");       // Direct string comparison
        CHECK(snd(result.value()) == "abc123"); // Direct string comparison
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum1("");
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum1("!@#abc123");
        CHECK_FALSE(result); // Should fail as the first character is not alphanumeric
    }
}
TEST_CASE("Alt parser works correctly", "[Alt]")
{
    SECTION("Alternative with character matches")
    {
        auto result = alt(ch('!'), tag("<tag>"))("!value");
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()) == "value");
            CHECK(snd(result.value()) == "!");
        }
    }

    SECTION("Alternative with tag matches")
    {
        auto result = alt(ch('!'), tag("<tag>"))("<tag>value");
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()) == "value");
            CHECK(snd(result.value()) == "<tag>");
        }
    }

    SECTION("No alternatives match")
    {
        auto result = alt(ch('!'), tag("<tag>"))("value");
        CHECK_FALSE(result);
    }
}

TEST_CASE("Alt parser with nested alternatives works correctly", "[Alt]")
{
    SECTION("Nested alternatives with multiple matching parsers")
    {
        auto nested_alt = alt(ch('a'), alt(ch('b'), tag("tag")), numeric1);

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

TEST_CASE("seq parser combinator works correctly", "[seq]")
{
    auto seq_parser = seq(ch('a'), ch('b'), numeric1);

    SECTION("All parsers match in sequence")
    {
        auto result = seq_parser("ab12345");
        CHECK(result);
        if (result)
        {
            auto res = snd(result.value());
            CHECK(p<0>(res) == "a");
            CHECK(p<1>(res) == "b");
            CHECK(p<2>(res) == "12345");
        }
    }

    SECTION("First parser fails")
    {
        auto result = seq_parser("zb12345");
        CHECK_FALSE(result);
    }

    SECTION("Middle parser fails")
    {
        auto result = seq_parser("a12345");
        CHECK_FALSE(result);
    }

    SECTION("Last parser fails")
    {
        auto result = seq_parser("ababc");
        CHECK_FALSE(result);
    }

    SECTION("Empty input")
    {
        auto result = seq_parser("");
        CHECK_FALSE(result);
    }
}

#endif