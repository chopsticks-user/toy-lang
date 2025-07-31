#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "token_stream.hpp"
#include "location_tracker.hpp"
#include "parse_error.hpp"

namespace tlc::parse {
    using ParserCombinatorResult = Expected<
        token::TokenizedBuffer, Error<EParseErrorContext, EParseErrorReason>
    >;

    using ParserCombinator = Fn<
        ParserCombinatorResult(TokenStream& stream, LocationTracker& tracker)
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [=]([[maybe_unused]] TokenStream& stream, \
    [[maybe_unused]] LocationTracker& tracker) \
        -> ParserCombinatorResult

    auto match(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(types...)) {
                return Unexpected{
                    Error<EParseErrorContext, EParseErrorReason>{}
                };
            }
            return {{stream.current()}};
        };
    }

    inline auto match(TokenStream::MatchFn const cond) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(cond)) {
                return Unexpected{
                    Error<EParseErrorContext, EParseErrorReason>{}
                };
            }
            return {{stream.current()}};
        };
    }

    inline auto many0(ParserCombinator const& pc) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer tokens;
            auto result = pc(stream, tracker);
            while (result) {
                tokens.append_range(*result);
                result = pc(stream, tracker);
            }
            return tokens;
        };
    }

    inline auto many1(ParserCombinator const& pc) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (ParserCombinatorResult const result =
                    many0(pc)(stream, tracker);
                !result.value().empty()) {
                return result;
            }
            return Unexpected{
                Error<EParseErrorContext, EParseErrorReason>{}
            };
        };
    }

    auto any(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            for (auto&& p : {pc...}) {
                if (auto const result = p(stream, tracker); result) {
                    return result;
                }
            }
            return Unexpected{
                Error<EParseErrorContext, EParseErrorReason>{}
            };
        };
    }

    auto seq(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer tokens;
            stream.markBacktrack();
            for (auto&& p : {pc...}) {
                auto const result = p(stream, tracker);
                if (!result) {
                    stream.backtrack();
                    return Unexpected{
                        Error<EParseErrorContext, EParseErrorReason>{}
                    };
                }
                tokens.append_range(result.value());
            }

            return tokens;
        };
    }
}

#endif // TLC_PARSE_COMBINATOR_HPP
