#ifndef EFP_PARSER_HPP_
#define EFP_PARSER_HPP_

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "prelude.hpp"
#include "string.hpp"

namespace efp
{
    // Parser should be static

    namespace parser
    {
        template <typename In, typename T>
        bool start_with(const Seq<In> &in, const Seq<T> &t)
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

        // Terminal parsers

        struct Ch
        {
            const char c;

            template <typename In>
            Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
            {
                if (length(in) > 0)
                {
                    if (in[0] == c)
                        return tuple(drop(1, in), take(1, in));
                    else
                        return nothing;
                }
                return nothing;
            }
        };

        Ch ch(char c)
        {
            return Ch{c};
        }

        // #define EFP_PARSER_TAG(name, str)                                                       \
//     struct name                                                                         \
//     {                                                                                   \
//         using namespace efp;                                                            \
//                                                                                         \
//         static StringView tag_str;                                                      \
//                                                                                         \
//         template <typename In>                                                          \
//         static Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)        \
//         {                                                                               \
//             if (length(in) > 0)                                                         \
//             {                                                                           \
//                 if (parser::start_with(in, tag_str))                                    \
//                     return tuple(drop(length(tag_str), in), take(length(tag_str), in)); \
//                 else                                                                    \
//                     return nothing;                                                     \
//             }                                                                           \
//             return nothing;                                                             \
//         }                                                                               \
//     };                                                                                  \
//     StringView name::tag_str = StringView(str, strlen(str));

        struct Tag
        {
            StringView t;

            template <typename In>
            Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
            {
                if (start_with(in, t))
                    return tuple(drop(length(t), in), t);
                else
                    return nothing;
            }
        };

        Tag tag(const char *str)
        {
            return Tag{StringView{str, strlen(str)}};
        }

        // alpha

        template <typename In>
        Maybe<Pair<StringView, StringView>> alpha0(const Seq<In> &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }

            // Return the consumed characters and the rest of the input
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                // Return the whole input and an empty string since no characters were consumed
                return tuple(drop(0, in), StringView{in.data(), 0});
        }

        // alpha1 will parse one or more alphabetic characters
        template <typename In>
        Maybe<Pair<StringView, StringView>> alpha1(const Seq<In> &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }

            // Must have consumed at least one character to succeed
            if (i > 0)
                return tuple(drop(i, in), take(i, in));
            else
                // If no characters were consumed, return nothing to indicate failure
                return nothing;
        }

        // numeric0 will parse zero or more numeric characters
        template <typename In>
        Maybe<Pair<StringView, StringView>> numeric0(const Seq<In> &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isdigit(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // numeric1 will parse one or more numeric characters
        template <typename In>
        Maybe<Pair<StringView, StringView>> numeric1(const Seq<In> &in)
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

        // alphanum0 will parse zero or more alphanumeric characters
        template <typename In>
        Maybe<Pair<StringView, StringView>> alphanum0(const Seq<In> &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isalnum(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // alphanum1 will parse one or more alphanumeric characters
        template <typename In>
        Maybe<Pair<StringView, StringView>> alphanum1(const Seq<In> &in)
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

        // Tag

        // struct Tag
        // {
        //     StringView t;

        //     template <typename In>
        //     Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
        //     {
        //         if (start_with(in, t))
        //             return tuple(drop(length(t), in), t);
        //         else
        //             return nothing;
        //     }
        // };

        // Tag tag(const char *str)
        // {
        //     return Tag{StringView{str, strlen(str)}};
        // }

        // template <StringView tag>
        // struct Tag
        // {
        //     // static constexpr StringView tag = {t, strlen(t)};

        //     template <typename In>
        //     Maybe<Pair<StringView, StringView>> operator()(const Seq<In> &in)
        //     {
        //         if (start_with(in, t))
        //             return tuple(drop(strlen(tag), in), tag);
        //         else
        //             return nothing;
        //     }
        // };

#define efp_tag_parser(x)

        // template <const char *str>
        // Tag<str> tag(const char *str)
        // {
        //     return Tag{StringView{str, strlen(str)}};
        // }

        // Alt

        template <typename... Ps>
        struct Alt
        {
            Tuple<Ps...> ps;

            template <size_t n, typename In, typename = EnableIf<(n < sizeof...(Ps))>>
            auto impl(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                const auto res = get<n>(ps)(in);

                if (res)
                    return res;
                else
                    return impl<n + 1>(in);
            }

            // Base case: when n equals the size of the tuple, stop recursion.
            template <size_t n, typename In, typename = EnableIf<(n >= sizeof...(Ps))>, typename = void>
            auto impl(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                return nothing; // Or some representation of failure
            }

            template <typename In>
            auto operator()(const Seq<In> &in) -> Maybe<Pair<StringView, StringView>>
            {
                return impl<0>(in);
            }
        };

        template <typename... Ps>
        auto alt(const Ps &...ps)
            -> Alt<Ps...>
        {
            return Alt<Ps...>{tuple(ps...)};
        }

    };
};

#endif

#endif