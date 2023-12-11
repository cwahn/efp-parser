#ifndef EFP_PARSER_COMBINATOR_HPP_
#define EFP_PARSER_COMBINATOR_HPP_

#include "parser_base.hpp"

namespace efp
{
    namespace parser
    {

        // Parser combinators
        template <typename... Ps>
        struct AltParser
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

            auto operator()(const Common<ParserI<Ps>...> &in) -> Common<CallReturn<Ps, Common<ParserI<Ps>...>>...>
            {
                return impl<0>(in);
            }
        };

        template <typename... Ps>
        auto alt(const Ps &...ps)
            -> AltParser<FuncToFuncPtr<Ps>...>
        {
            return AltParser<FuncToFuncPtr<Ps>...>{tuple(ps...)};
        }

        // TupleParser
        // Basic sequential parser

        namespace detail
        {
            template <size_t n, typename Tuple, typename In, typename... Results>
            struct TupleParserImpl
            {
            };

            template <size_t n, typename In, typename... Ps, typename... Results>
            struct TupleParserImpl<n, Tuple<Ps...>, In, Results...>
            {
                static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
                    -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
                {
                    const auto res = get<n>(t)(in);

                    if (!res)
                        return nothing; // If any parser fails, return nothing
                    else
                        // Recursive call with the next index
                        return TupleParserImpl<n + 1, Tuple<Ps...>, In, Results..., decltype(snd(res.value()))>::process(
                            t, fst(res.value()), results..., snd(res.value()));
                }
            };

            // Specialization for the base case
            template <typename In, typename... Ps, typename... Results>
            struct TupleParserImpl<sizeof...(Ps), Tuple<Ps...>, In, Results...>
            {
                static auto process(const Tuple<Ps...> &t, const In &in, Results... results)
                    -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
                {
                    return tuple(in, tuple(results...)); // Returning the accumulated results
                }
            };
        }

        template <typename... Ps>
        struct TupleParser
        {
            Tuple<Ps...> ps;

            auto operator()(const Common<ParserI<Ps>...> &in) const
                -> Parsed<Common<ParserI<Ps>...>, Tuple<ParserO<Ps>...>>
            {
                return detail::TupleParserImpl<0, Tuple<Ps...>, Common<ParserI<Ps>...>>::process(ps, in);
            }
        };

        template <typename... Ps>
        auto tpl(const Ps &...ps)
            -> TupleParser<FuncToFuncPtr<Ps>...>
        {
            return TupleParser<FuncToFuncPtr<Ps>...>{tuple(ps...)};
        }
    }

}

#endif