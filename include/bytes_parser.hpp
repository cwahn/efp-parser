#ifndef EFP_BYTE_PARSER_HPP
#define EFP_BYTE_PARSER_HPP

namespace efp
{
    namespace parser
    {
        template <typename In>
        struct Tag
        {
            In t;

            auto operator()(const In &in) const
                -> Parsed<In, In>
            {
                if (start_with(in, t))
                    return tuple(drop(length(t), in), t);
                else
                    return nothing;
            }
        };

        template <>
        struct Tag<StringView>
        {
            StringView t;

            auto operator()(const StringView &in) const
                -> Parsed<StringView, StringView>
            {
                if (start_with(in, t))
                    return tuple(drop(length(t), in), t);
                else
                    return nothing;
            }
        };

        // template <typename In>
        // Tag<In> tag(const In &t)
        // {
        //     return Tag<In>{t};
        // }

        Tag<StringView> tag(const StringView &t)
        {
            return Tag<StringView>{t};
        }

    } // namespace parser

} // namespace efp

#endif