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
        CHECK(start_with(StringView{"hello", 5}, StringView{"he", 2}));
    }

    SECTION("Sequence does not start with the given prefix")
    {
        CHECK_FALSE(start_with(StringView{"hello", 5}, StringView{"lo", 2}));
    }
}

TEST_CASE("Ch parser works correctly", "[Ch]")
{
    // Ch c{'a'};

    SECTION("Sequence starts with the character")
    {
        auto result = ch('a')(StringView{"apple", 5});
        // auto result = Ch<'a'>{}(StringView{"apple", 5});

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 4); // "pple"
            CHECK(snd(result.value()).size() == 1); // "a"
        }
    }

    SECTION("Sequence does not start with the character")
    {
        auto result = ch('a')(StringView{"banana", 6});
        // auto result = Ch<'a'>{}(StringView{"banana", 6});

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
        auto result = alpha0(StringView{"abcde", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "abcde"
    }

    SECTION("Empty sequence")
    {
        auto result = alpha0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha0(StringView{"123abc", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 6); // Remaining input is "123abc"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("alpha1 parser works correctly", "[alpha1]")
{
    SECTION("Sequence with only alphabetic characters")
    {
        auto result = alpha1(StringView{"abcde", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "abcde"
    }

    SECTION("Empty sequence")
    {
        auto result = alpha1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha1(StringView{"123abc", 6});
        CHECK_FALSE(result); // Should fail as the first character is not alphabetic
    }
}

TEST_CASE("numeric0 parser works correctly", "[numeric0]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric0(StringView{"12345", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "12345"
    }

    SECTION("Empty sequence")
    {
        auto result = numeric0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric0(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 6); // Remaining input is "abc123"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("numeric1 parser works correctly", "[numeric1]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric1(StringView{"12345", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "12345"
    }

    SECTION("Empty sequence")
    {
        auto result = numeric1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric1(StringView{"abc123", 6});
        CHECK_FALSE(result); // Should fail as the first character is not numeric
    }
}

TEST_CASE("alphanum0 parser works correctly", "[alphanum0]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum0(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 6); // Parsed "abc123"
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum0(StringView{"!@#abc123", 9});
        CHECK(result);
        CHECK(fst(result.value()).size() == 9); // Remaining input is "!@#abc123"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("alphanum1 parser works correctly", "[alphanum1]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum1(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 6); // Parsed "abc123"
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum1(StringView{"!@#abc123", 9});
        CHECK_FALSE(result); // Should fail as the first character is not alphanumeric
    }
}

TEST_CASE("Alt parser works correctly", "[Alt]")
{
    // Alt<Ch, Tag> alt{tuple(Ch{'!'}, Tag{StringView{"<tag>", 5}})};

    SECTION("Alternative with character matches")
    {
        // const char *tag_cstr = "<tag>";
        // auto result = alt(Ch<'!'>{}, Tag<"<tag>">{})(StringView{"!value", 6});
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"!value", 6});

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 1); // "!"
        }
    }

    SECTION("Alternative with tag matches")
    {
        // const char *tag_cstr = "<tag>";
        // auto result = alt(Ch<'!'>{}, Tag<"<tag>">{})(StringView{"<tag>value", 10});
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"<tag>value", 10});

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 5); // "<tag>"
        }
    }

    SECTION("No alternatives match")
    {
        // const char *tag_cstr = "<tag>";
        // auto result = alt(Ch<'!'>{}, Tag<"<tag>">{})(StringView{"value", 5});
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"value", 5});

        CHECK_FALSE(result);
    }
}

TEST_CASE("Alt parser with nested alternatives works correctly", "[Alt]")
{
    SECTION("Nested alternatives with multiple matching parsers")
    {
        // Define some simple parsers
        auto a_parser = ch('a');
        auto b_parser = ch('b');
        auto tag_parser = tag("tag");

        // Create a nested alternative parser
        auto nested_alt = alt(a_parser, alt(b_parser, tag_parser), numeric1);

        // Test case where the first parser matches
        {
            auto result = nested_alt(StringView{"apple", 5});
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()).size() == 4); // "pple"
                CHECK(snd(result.value()).size() == 1); // "a"
            }
        }

        // Test case where the nested alternative matches
        {
            auto result = nested_alt(StringView{"tagvalue", 8});
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()).size() == 5); // "value"
                CHECK(snd(result.value()).size() == 3); // "tag"
            }
        }

        // Test case where the last parser matches
        {
            auto result = nested_alt(StringView{"1234", 4});
            CHECK(result);
            if (result)
            {
                CHECK(fst(result.value()).size() == 0); // Empty (all input consumed)
                CHECK(snd(result.value()).size() == 4); // "1234"
            }
        }

        // Test case where no parsers match
        {
            auto result = nested_alt(StringView{"!none", 5});
            CHECK_FALSE(result);
        }
    }
}

TEST_CASE("tpl parser combinator works correctly", "[tpl]")
{
    auto a_parser = ch('a');
    auto b_parser = ch('b');
    auto numeric_parser = numeric1;

    auto tpl_parser = tpl(a_parser, b_parser, numeric_parser);

    SECTION("All parsers match in sequence")
    {
        auto result = tpl_parser(StringView{"ab12345", 7});
        CHECK(result);
        if (result)
        {
            auto res = snd(result.value());
            CHECK(p<0>(res).size() == 1); // "a"
            CHECK(p<1>(res).size() == 1); // "b"
            CHECK(p<2>(res).size() == 5); // "12345"
        }
    }

    SECTION("First parser fails")
    {
        auto result = tpl_parser(StringView{"zb12345", 7});
        CHECK_FALSE(result);
    }

    SECTION("Middle parser fails")
    {
        auto result = tpl_parser(StringView{"a12345", 6});
        CHECK_FALSE(result);
    }

    SECTION("Last parser fails")
    {
        auto result = tpl_parser(StringView{"ababc", 5});
        CHECK_FALSE(result);
    }

    SECTION("Empty input")
    {
        auto result = tpl_parser(StringView{"", 0});
        CHECK_FALSE(result);
    }
}

#endif