#ifndef EFP_TERMINAL_PARSER_HPP_
#define EFP_TERMINAL_PARSER_HPP_

#include "parser_base.hpp"

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
        struct ChParser
        {
            const char c;

            Parsed<StringView, char> operator()(const StringView &in) const
            {
                if (length(in) > 0)
                {
                    if (in[0] == c)
                        return tuple(drop(1, in), c);
                    else
                        return nothing;
                }
                return nothing;
            }
        };

        ChParser ch(char c)
        {
            return ChParser{c};
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

        // one_of: Recognizes one of the provided characters
        struct OneOfParser
        {
            StringView chars_to_match;

            OneOfParser(const char *chars)
                : chars_to_match(StringView(chars)) {}

            Parsed<StringView, char> operator()(const StringView &in) const
            {
                if (length(in) > 0 && elem_index(in[0], chars_to_match))
                    return tuple(drop(1, in), in[0]);
                else
                    return nothing;
            }
        };

        auto one_of(const char *chars) -> OneOfParser
        {
            return OneOfParser(chars);
        }

        Parsed<StringView, int8_t> parse_int8(const StringView &in)
        {
            int temp;
            if (sscanf(in.data(), "%d", &temp) == 1 && temp >= std::numeric_limits<int8_t>::min() && temp <= std::numeric_limits<int8_t>::max())
            {
                return tuple(drop_while(isdigit, in), static_cast<int8_t>(temp));
            }
            return nothing;
        }

        Parsed<StringView, int16_t> parse_int16(const StringView &in)
        {
            int temp;
            if (sscanf(in.data(), "%d", &temp) == 1 && temp >= std::numeric_limits<int16_t>::min() && temp <= std::numeric_limits<int16_t>::max())
            {
                return tuple(drop_while(isdigit, in), static_cast<int16_t>(temp));
            }
            return nothing;
        }

        Parsed<StringView, int32_t> parse_int32(const StringView &in)
        {
            int32_t temp;
            if (sscanf(in.data(), "%d", &temp) == 1)
            {
                return tuple(drop_while(isdigit, in), temp);
            }
            return nothing;
        }

        Parsed<StringView, int64_t> parse_int64(const StringView &in)
        {
            int64_t temp;
            if (sscanf(in.data(), "%lld", &temp) == 1)
            {
                return tuple(drop_while(isdigit, in), temp);
            }
            return nothing;
        }

        Parsed<StringView, uint8_t> parse_uint8(const StringView &in)
        {
            unsigned int temp;
            if (sscanf(in.data(), "%u", &temp) == 1 && temp <= std::numeric_limits<uint8_t>::max())
            {
                return tuple(drop_while(isdigit, in), static_cast<uint8_t>(temp));
            }
            return nothing;
        }

        Parsed<StringView, uint16_t> parse_uint16(const StringView &in)
        {
            unsigned int temp;
            if (sscanf(in.data(), "%u", &temp) == 1 && temp <= std::numeric_limits<uint16_t>::max())
            {
                return tuple(drop_while(isdigit, in), static_cast<uint16_t>(temp));
            }
            return nothing;
        }

        Parsed<StringView, uint32_t> parse_uint32(const StringView &in)
        {
            uint32_t temp;
            if (sscanf(in.data(), "%u", &temp) == 1)
            {
                return tuple(drop_while(isdigit, in), temp);
            }
            return nothing;
        }

        Parsed<StringView, uint64_t> parse_uint64(const StringView &in)
        {
            uint64_t temp;
            if (sscanf(in.data(), "%llu", &temp) == 1)
            {
                return tuple(drop_while(isdigit, in), temp);
            }
            return nothing;
        }

        // satisfy: Recognizes one character and checks that it satisfies a predicate
        template <typename Predicate>
        struct SatisfyParser
        {
            Predicate pred;

            explicit SatisfyParser(Predicate p) : pred(p) {}

            Parsed<StringView, char> operator()(const StringView &in) const
            {
                if (length(in) > 0 && pred(in[0]))
                    return tuple(drop(1, in), in[0]);
                else
                    return nothing;
            }
        };

        // Constructor function for SatisfyParser
        template <typename Predicate>
        auto satisfy(Predicate p) -> SatisfyParser<Predicate>
        {
            return SatisfyParser<Predicate>(p);
        }

        // space0: Parses zero or more space characters
        auto space0(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && in[i] == ' ')
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // space1: Parses one or more space characters
        auto space1(const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && in[i] == ' ')
            {
                ++i;
            }
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                return nothing;
        }

        // tab: Matches a tab character ‘\t’
        auto tab(const StringView &in) -> Parsed<StringView, char>
        {
            if (!in.empty() && in[0] == '\t')
                return tuple(drop(1, in), '\t');
            else
                return nothing;
        }

    }

    
}

#endif