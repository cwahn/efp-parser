#ifndef EFP_TERMINAL_PARSER_HPP_
#define EFP_TERMINAL_PARSER_HPP_

#include "prelude.hpp"
#include "string.hpp"

// alpha0/alpha1: Parses zero or more, or one or more alphabetic characters.
// alphanumeric0/alphanumeric1: Parses zero or more, or one or more alphanumeric characters.
// anychar: Matches any single character.
// char: Recognizes a specific character.
// crlf: Matches the string "\r\n".
// digit0/digit1: Parses zero or more, or one or more numeric characters.
// hex_digit0/hex_digit1: Parses zero or more, or one or more hexadecimal digits.
// i8/i16/i32/i64/i128, u8/u16/u32/u64/u128: Parses various integer sizes.
// line_ending: Recognizes an end of line.
// multispace0/multispace1: Parses zero or more, or one or more whitespace characters.
// newline: Matches a newline character.
// none_of: Recognizes any character not in the provided list.
// not_line_ending: Parses any characters except line endings.
// oct_digit0/oct_digit1: Parses zero or more, or one or more octal digits.
// one_of: Recognizes one of the provided characters.
// satisfy: Matches a character that satisfies a predicate.
// space0/space1: Parses zero or more, or one or more space characters.
// tab: Matches a tab character.

namespace efp
{
    namespace parser
    {
        // Maybe pair of remaining and matched
        template <typename I, typename O>
        using Parsed = Maybe<Pair<I, O>>;

        template <typename P>
        using ParserI = TupleAt<0, EnumAt<1, Return<P>>>;

        template <typename P>
        using ParserO = TupleAt<1, EnumAt<1, Return<P>>>;

        bool start_with(const StringView &in, const StringView &t)
        {
            const auto t_length = length(t);

            if (length(in) >= t_length)
            {
                for (size_t i = 0; i < t_length; ++i)
                {
                    if (in[i] != t[i])
                        return false;
                }
                return true;
            }
            return false;
        }

        // Function alpha0: Parses zero or more alphabetic characters
        auto alpha0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // Function alpha1: Parses one or more alphabetic characters
        auto alpha1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // alphanumeric0: Parses zero or more alphanumeric characters
        auto alphanumeric0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isalnum(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // alphanumeric1: Parses one or more alphanumeric characters
        auto alphanumeric1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isalnum(in[i]))
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // anychar: Matches any single character
        auto anychar(const StringView &in) -> Parsed<StringView, char>
        {
            if (length(in) > 0)
                return tuple(drop(1, in), in[0]);
            else
                return nothing;
        }

        // ch: Recognizes a specific character
        auto ch(char target) -> std::function<Parsed<StringView, char>(const StringView &)>
        {
            return [target](const StringView &in) -> Parsed<StringView, char>
            {
                if (length(in) > 0 && in[0] == target)
                    return tuple(drop(1, in), in[0]);
                else
                    return nothing;
            };
        }

        // crlf: Matches the string "\r\n"
        auto crlf(const StringView &in) -> Parsed<StringView, StringView>
        {
            if (length(in) >= 2 && in[0] == '\r' && in[1] == '\n')
                return tuple(drop(2, in), take(2, in));
            else
                return nothing;
        }

        // digit0: Parses zero or more numeric characters
        auto digit0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isdigit(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // digit1: Parses one or more numeric characters
        auto digit1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isdigit(in[i]))
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // hex_digit0: Parses zero or more hexadecimal digits
        auto hex_digit0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isxdigit(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // hex_digit1: Parses one or more hexadecimal digits
        auto hex_digit1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isxdigit(in[i]))
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // Parses a 32-bit signed integer
        // auto int_parser(const StringView &in) -> Parsed<StringView, int>
        // {
        //     auto res = digit1(in); // Use digit1 to ensure at least one digit
        //     if (!res)
        //         return nothing;

        //     int number;
        //     if (std::from_chars(snd(res.value()).data(), snd(res.value()).data() + snd(res.value()).size(), number).ec == std::errc())
        //     {
        //         return tuple(fst(res.value()), number);
        //     }
        //     return nothing;
        // }

        // // Parses a 32-bit unsigned integer
        // auto uint_parser(const StringView &in) -> Parsed<StringView, uint>
        // {
        //     auto res = digit1(in); // Use digit1 to ensure at least one digit
        //     if (!res)
        //         return nothing;

        //     uint number;
        //     if (std::from_chars(snd(res.value()).data(), snd(res.value()).data() + snd(res.value()).size(), number).ec == std::errc())
        //     {
        //         return tuple(fst(res.value()), number);
        //     }
        //     return nothing;
        // }

        // line_ending: Recognizes an end of line (both ‘\n’ and ‘\r\n’).
        auto line_ending(const StringView &in) -> Parsed<StringView, StringView>
        {
            if (start_with(in, "\r\n"))
                return tuple(drop(2, in), take(2, in));
            else if (start_with(in, "\n"))
                return tuple(drop(1, in), take(1, in));
            else
                return nothing;
        }

        // multispace0: Recognizes zero or more whitespace characters
        auto multispace0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isspace(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // multispace1: Recognizes one or more whitespace characters
        auto multispace1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isspace(in[i]))
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // newline: Matches a newline character ‘\n’
        auto newline(const StringView &in) -> Parsed<StringView, char>
        {
            if (!in.empty() && in[0] == '\n')
                return tuple(drop(1, in), in[0]);
            else
                return nothing;
        }

        // none_of: Recognizes a character that is not in the provided characters
        struct NoneOfParser
        {
            StringView chars_to_avoid;

            NoneOfParser(const char *chars)
                : chars_to_avoid(StringView(chars)) {}

            Parsed<StringView, char> operator()(const StringView &in) const
            {
                if (length(in) > 0 && !elem_index(in[0], chars_to_avoid))
                    return tuple(drop(1, in), in[0]);
                else
                    return nothing;
            }
        };

        auto none_of(const char *chars) -> NoneOfParser
        {
            return NoneOfParser(chars);
        }

        // not_line_ending: Recognizes a string of any char except ‘\r\n’ or ‘\n’.
        auto not_line_ending(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && in[i] != '\n' && in[i] != '\r')
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // oct_digit0: Parses zero or more octal characters (0-7)
        auto oct_digit0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && in[i] >= '0' && in[i] <= '7')
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // oct_digit1: Parses one or more octal characters (0-7)
        auto oct_digit1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && in[i] >= '0' && in[i] <= '7')
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

    }
}

#endif