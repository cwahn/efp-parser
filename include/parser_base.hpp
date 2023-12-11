#ifndef EFP_PARSER_BASE_HPP_
#define EFP_PARSER_BASE_HPP_

#include "prelude.hpp"
#include "string.hpp"

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

        // ? CRTP interface?
        template <typename In>
        bool start_with(const In &in, const In &t)
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
    }
}

#endif