#ifndef EFP_PARSER_HPP_
#define EFP_PARSER_HPP_

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

            Maybe<Pair<StringView, StringView>> operator()(const StringView &in)
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

        struct Tag
        {
            StringView t;

            Maybe<Pair<StringView, StringView>> operator()(const StringView &in)
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

        // Maybe<Pair<StringView, StringView>> alpha0(const StringView &in)
        // {
        //     size_t i = 0;
        //     while (i < length(in) && std::isalpha(in[i]))
        //     {
        //         ++i;
        //     }

        //     // Return the consumed characters and the rest of the input
        //     if (i > 0)
        //         return tuple(drop(i, in), take(i, in));
        //     else
        //         // Return the whole input and an empty string since no characters were consumed
        //         return tuple(drop(0, in), StringView{in.data(), 0});
        // }

        auto alpha0 = [](const auto &in) -> Maybe<Pair<StringView, StringView>>
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }
            return i > 0 ? tuple(drop(i, in), take(i, in)) : tuple(drop(0, in), StringView{in.data(), 0});
        };

        // alpha1 will parse one or more alphabetic characters

        // Maybe<Pair<StringView, StringView>> alpha1(const StringView &in)
        // {
        //     size_t i = 0;
        //     while (i < length(in) && std::isalpha(in[i]))
        //     {
        //         ++i;
        //     }

        //     // Must have consumed at least one character to succeed
        //     if (i > 0)
        //         return tuple(drop(i, in), take(i, in));
        //     else
        //         // If no characters were consumed, return nothing to indicate failure
        //         return nothing;
        // }
        auto alpha1 = [](const auto &in) -> Maybe<Pair<StringView, StringView>>
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
        };

        // numeric0 will parse zero or more numeric characters

        // Maybe<Pair<StringView, StringView>> numeric0(const StringView &in)
        // {
        //     size_t i = 0;
        //     while (i < length(in) && std::isdigit(in[i]))
        //     {
        //         ++i;
        //     }
        //     return tuple(drop(i, in), take(i, in));
        // }
        auto numeric0 = [](const auto &in) -> Maybe<Pair<StringView, StringView>>
        {
            size_t i = 0;
            while (i < length(in) && std::isdigit(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        };

        // numeric1 will parse one or more numeric characters

        // Maybe<Pair<StringView, StringView>> numeric1(const StringView &in)
        // {
        //     size_t i = 0;
        //     while (i < length(in) && std::isdigit(in[i]))
        //     {
        //         ++i;
        //     }

        //     if (i > 0)
        //         return tuple(drop(i, in), take(i, in));
        //     else
        //         return nothing;
        // }

        auto numeric1 = [](const auto &in) -> Maybe<Pair<StringView, StringView>>
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
        };

        // alphanum0 will parse zero or more alphanumeric characters

        Maybe<Pair<StringView, StringView>> alphanum0(const StringView &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isalnum(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // alphanum1 will parse one or more alphanumeric characters

        Maybe<Pair<StringView, StringView>> alphanum1(const StringView &in)
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

        // Parser combinators

        template <typename... Ps>
        struct Alt
        {
            Tuple<Ps...> ps;

            template <size_t n, typename In, typename = EnableIf<(n < sizeof...(Ps))>>
            auto impl(const In &in) -> Common<CallReturn<Ps, In>...>
            {
                const auto res = get<n>(ps)(in);

                if (res)
                    return res;
                else
                    return impl<n + 1>(in);
            }

            // Base case: when n equals the size of the tuple, stop recursion.
            template <size_t n, typename In, typename = EnableIf<(n >= sizeof...(Ps))>, typename = void>
            auto impl(const In &in) -> Common<CallReturn<Ps, In>...>
            {
                return nothing; // Or some representation of failure
            }

            template <typename In>
            auto operator()(const In &in) -> Common<CallReturn<Ps, In>...>
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
