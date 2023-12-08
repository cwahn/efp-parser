#ifndef EFP_PARSER_HPP_
#define EFP_PARSER_HPP_

#include "prelude.hpp"
#include "string.hpp"

namespace efp
{
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

        // Maybe pair of remaining and matched
        template <typename I, typename O>
        using Parsed = Maybe<Pair<I, O>>;

        template <typename P>
        using ParserI = TupleAt<0, EnumAt<1, Return<P>>>;

        template <typename P>
        using ParserO = TupleAt<1, EnumAt<1, Return<P>>>;

        // Terminal parsers

        struct Ch
        {
            const char c;

            Parsed<StringView, StringView> operator()(const StringView &in) const
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

            Parsed<StringView, StringView> operator()(const StringView &in) const
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

        auto alpha0 = [](const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isalpha(in[i]))
            {
                ++i;
            }
            return i > 0 ? tuple(drop(i, in), take(i, in)) : tuple(drop(0, in), StringView{in.data(), 0});
        };

        // alpha1 will parse one or more alphabetic characters

        auto alpha1 = [](const StringView &in) -> Parsed<StringView, StringView>
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

        auto numeric0 = [](const StringView &in) -> Parsed<StringView, StringView>
        {
            size_t i = 0;
            while (i < length(in) && std::isdigit(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        };

        // numeric1 will parse one or more numeric characters

        auto numeric1 = [](const StringView &in) -> Parsed<StringView, StringView>
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

        Parsed<StringView, StringView> alphanum0(const StringView &in)
        {
            size_t i = 0;
            while (i < length(in) && std::isalnum(in[i]))
            {
                ++i;
            }
            return tuple(drop(i, in), take(i, in));
        }

        // alphanum1 will parse one or more alphanumeric characters

        Parsed<StringView, StringView> alphanum1(const StringView &in)
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

        // Tpl
        // Basic sequential parser

        template <size_t n, typename Tuple, typename In, typename... Results>
        struct TupleProcessor
        {
        };

        template <size_t n, typename In, typename... Ps, typename... Results>
        struct TupleProcessor<n, Tuple<Ps...>, In, Results...>
        {
            static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
                -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
            {
                const auto res = get<n>(t)(in);

                if (!res)
                    return nothing; // If any parser fails, return nothing
                else
                    // Recursive call with the next index
                    return TupleProcessor<n + 1, Tuple<Ps...>, In, Results..., decltype(snd(res.value()))>::process(
                        t, fst(res.value()), results..., snd(res.value()));
            }
        };

        // Specialization for the base case
        template <typename In, typename... Ps, typename... Results>
        struct TupleProcessor<sizeof...(Ps), Tuple<Ps...>, In, Results...>
        {
            static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
                -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
            {
                return tuple(in, tuple(results...)); // Returning the accumulated results
            }
        };

        template <typename... Ps>
        struct Tpl
        {
            Tuple<Ps...> ps;

            template <typename In>
            auto operator()(const In &in) const
                -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
            {
                return TupleProcessor<0, Tuple<Ps...>, In>::process(ps, in);
            }
        };

        // template <typename... Ps>
        // struct Tpl
        // {
        //     Tuple<Ps...> ps;

        //     template <size_t n>
        //     struct OperatorImpl
        //     {
        //         template <typename In, typename... Results>
        //         static auto impl(const In &in, Results... results)
        //             -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //         {
        //             const auto res = get<n>(ps)(in);

        //             if (!res)
        //                 return nothing; // If any parser fails, return nothing
        //             else
        //                 return impl<n + 1>(fst(res.value()), results..., snd(res.value()));
        //         }
        //     };

        //     template <>
        //     struct OperatorImpl<sizeof...(Ps)>
        //     {
        //         template <typename In, typename... Results>
        //         static auto impl(const In &in, Results... results)
        //             -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //         {
        //             return tuple(results...); // Return the accumulated results as a tuple
        //         }
        //     };

        //     template <typename In>
        //     auto operator()(const In &in)
        //         -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //     {
        //         return OperatorImpl::impl<0>(in);
        //     }
        // };

        template <typename... Ps>
        auto tpl(const Ps &...ps) -> Tpl<Ps...>
        {
            return Tpl<Ps...>{tuple(ps...)};
        }
    };
};

#endif
