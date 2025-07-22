#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    // todo: move error.hpp
    struct Error {};

    using Result = Expected<
        token::TokenizedBuffer, Error
    >;

    using ParserCombinator = Fn<
        Result(
            Context& context, TokenStream& stream, Panic& panic
        )
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [=]([[maybe_unused]] Context& context, [[maybe_unused]] TokenStream& stream, [[maybe_unused]] Panic& panic) \
        -> Result

    auto match(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(types...)) {
                return Unexpected{Error{}};
            }
            return {{stream.current()}};
        };
    }

    inline auto match(TokenStream::MatchFn const cond) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(cond)) {
                return Unexpected{Error{}};
            }
            return {{stream.current()}};
        };
    }

    inline auto many0(ParserCombinator const& pc) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer tokens;
            while (pc(context, stream, panic)
                   .transform([&tokens](auto v)-> void {
                       tokens.append_range(v);
                   }).has_value()) {}
            return tokens;
        };
    }

    inline auto many1(ParserCombinator const& pc) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            // return (match(type) & (*type))(context, stream, panic);

            if (Result const result = many0(pc)(context, stream, panic);
                !result.value().empty()) {
                return result;
            }
            return Unexpected{Error{}};
        };
    }

    auto any(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            for (auto&& p : {pc...}) {
                if (auto const result = p(context, stream, panic); result) {
                    return result;
                }
            }
            return Unexpected{Error{}};
        };
    }

    auto seq(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer tokens;
            stream.markBacktrack();
            for (auto&& p : {pc...}) {
                auto const result = p(context, stream, panic);
                if (!result) {
                    stream.backtrack();
                    return Unexpected{Error{}};
                }
                tokens.append_range(result.value());
            }

            return tokens;
        };
    }
}

#endif // TLC_PARSE_COMBINATOR_HPP
