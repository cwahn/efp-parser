#ifndef EFP_PARSER_HPP_
#define EFP_PARSER_HPP_

#include "prelude.hpp"
#include "string.hpp"
#include "character_parser.hpp"
#include "bytes_parser.hpp"
#include "parser_combinator.hpp"

namespace efp
{
    namespace parser
    {
        //     bool start_with(const StringView &in, const StringView &t)
        //     {
        //         const auto t_length = length(t);

        //         if (length(in) >= t_length)
        //         {
        //             for (size_t i = 0; i < t_length; ++i)
        //             {
        //                 if (in[i] != t[i])
        //                     return false;
        //             }
        //             return true;
        //         }
        //         return false;
        //     }

        //     // Maybe pair of remaining and matched
        //     template <typename I, typename O>
        //     using Parsed = Maybe<Pair<I, O>>;

        //     template <typename P>
        //     using ParserI = TupleAt<0, EnumAt<1, Return<P>>>;

        //     template <typename P>
        //     using ParserO = TupleAt<1, EnumAt<1, Return<P>>>;

        //     // Terminal parsers

        //     struct Ch
        //     {
        //         const char c;

        //         Parsed<StringView, StringView> operator()(const StringView &in) const
        //         {
        //             if (length(in) > 0)
        //             {
        //                 if (in[0] == c)
        //                     return tuple(drop(1, in), take(1, in));
        //                 else
        //                     return nothing;
        //             }
        //             return nothing;
        //         }
        //     };

        //     Ch ch(char c)
        //     {
        //         return Ch{c};
        //     }

        //     struct Tag
        //     {
        //         StringView t;

        //         Parsed<StringView, StringView> operator()(const StringView &in) const
        //         {
        //             if (start_with(in, t))
        //                 return tuple(drop(length(t), in), t);
        //             else
        //                 return nothing;
        //         }
        //     };

        //     Tag tag(const char *str)
        //     {
        //         return Tag{StringView{str, strlen(str)}};
        //     }

        //     // alpha
        //     auto alpha0(const StringView &in) -> Parsed<StringView, StringView>
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isalpha(in[i]))
        //         {
        //             ++i;
        //         }

        //         return tuple(drop(i, in), take(i, in));
        //     }

        //     // alpha1 will parse one or more alphabetic characters
        //     auto alpha1(const StringView &in) -> Parsed<StringView, StringView>
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isalpha(in[i]))
        //         {
        //             ++i;
        //         }

        //         if (i > 0)
        //             return tuple(drop(i, in), take(i, in));
        //         else
        //             return nothing;
        //     }

        //     // numeric0 will parse zero or more numeric characters
        //     auto numeric0(const StringView &in) -> Parsed<StringView, StringView>
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isdigit(in[i]))
        //         {
        //             ++i;
        //         }

        //         return tuple(drop(i, in), take(i, in));
        //     }

        //     // numeric1 will parse one or more numeric characters
        //     auto numeric1(const StringView &in) -> Parsed<StringView, StringView>
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isdigit(in[i]))
        //         {
        //             ++i;
        //         }

        //         if (i > 0)
        //             return tuple(drop(i, in), take(i, in));
        //         else
        //             return nothing;
        //     }

        //     // alphanum0 will parse zero or more alphanumeric characters
        //     Parsed<StringView, StringView> alphanum0(const StringView &in)
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isalnum(in[i]))
        //         {
        //             ++i;
        //         }
        //         return tuple(drop(i, in), take(i, in));
        //     }

        //     // alphanum1 will parse one or more alphanumeric characters
        //     Parsed<StringView, StringView> alphanum1(const StringView &in)
        //     {
        //         size_t i = 0;
        //         while (i < length(in) && std::isalnum(in[i]))
        //         {
        //             ++i;
        //         }

        //         if (i > 0)
        //             return tuple(drop(i, in), take(i, in));
        //         else
        //             return nothing;
        //     }

        //     // Parser combinators
        //     template <typename... Ps>
        //     struct AltParser
        //     {
        //         Tuple<Ps...> ps;

        //         template <size_t n, typename In, typename = EnableIf<(n < sizeof...(Ps))>>
        //         auto impl(const In &in) -> Common<CallReturn<Ps, In>...>
        //         {
        //             const auto res = get<n>(ps)(in);

        //             if (res)
        //                 return res;
        //             else
        //                 return impl<n + 1>(in);
        //         }

        //         // Base case: when n equals the size of the tuple, stop recursion.
        //         template <size_t n, typename In, typename = EnableIf<(n >= sizeof...(Ps))>, typename = void>
        //         auto impl(const In &in) -> Common<CallReturn<Ps, In>...>
        //         {
        //             return nothing; // Or some representation of failure
        //         }

        //         auto operator()(const Common<ParserI<Ps>...> &in) -> Common<CallReturn<Ps, Common<ParserI<Ps>...>>...>
        //         {
        //             return impl<0>(in);
        //         }
        //     };

        //     template <typename... Ps>
        //     auto alt(const Ps &...ps)
        //         -> AltParser<FuncToFuncPtr<Ps>...>
        //     {
        //         return AltParser<FuncToFuncPtr<Ps>...>{tuple(ps...)};
        //     }

        //     // TupleParser
        //     // Basic sequential parser

        //     namespace detail
        //     {
        //         template <size_t n, typename Tuple, typename In, typename... Results>
        //         struct TupleParserImpl
        //         {
        //         };

        //         template <size_t n, typename In, typename... Ps, typename... Results>
        //         struct TupleParserImpl<n, Tuple<Ps...>, In, Results...>
        //         {
        //             static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
        //                 -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //             {
        //                 const auto res = get<n>(t)(in);

        //                 if (!res)
        //                     return nothing; // If any parser fails, return nothing
        //                 else
        //                     // Recursive call with the next index
        //                     return TupleParserImpl<n + 1, Tuple<Ps...>, In, Results..., decltype(snd(res.value()))>::process(
        //                         t, fst(res.value()), results..., snd(res.value()));
        //             }
        //         };

        //         // Specialization for the base case
        //         template <typename In, typename... Ps, typename... Results>
        //         struct TupleParserImpl<sizeof...(Ps), Tuple<Ps...>, In, Results...>
        //         {
        //             static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
        //                 -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //             {
        //                 return tuple(in, tuple(results...)); // Returning the accumulated results
        //             }
        //         };
        //     }

        //     template <typename... Ps>
        //     struct TupleParser
        //     {
        //         Tuple<Ps...> ps;

        //         auto operator()(const Common<ParserI<Ps>...> &in) const
        //             -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
        //         {
        //             return detail::TupleParserImpl<0, Tuple<Ps...>, Common<ParserI<Ps>...>>::process(ps, in);
        //         }
        //     };

        //     template <typename... Ps>
        //     auto tpl(const Ps &...ps)
        //         -> TupleParser<FuncToFuncPtr<Ps>...>
        //     {
        //         return TupleParser<FuncToFuncPtr<Ps>...>{tuple(ps...)};
        //     }
        // };

        // many0

        // template <typename P>
        // struct Many0
        // {
        //     auto operator()()
        //     {

        //     }
    }
};

#endif
