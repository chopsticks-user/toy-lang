#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "token_stream.hpp"
#include "location_tracker.hpp"
#include "error_collector.hpp"

namespace tlc::parse {
    using ParserCombinatorResult = Expected<
        token::TokenizedBuffer, Error
    >;

    using ParserCombinator = Fn<
        ParserCombinatorResult(
            TokenStream& stream, LocationTracker& tracker, ErrorCollector& collector
        )
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [=]([[maybe_unused]] TokenStream& stream, \
    [[maybe_unused]] LocationTracker& tracker, \
    [[maybe_unused]] ErrorCollector& collector) \
        -> ParserCombinatorResult

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
            auto result = pc(stream, tracker, collector);
            while (result) {
                tokens.append_range(*result);
                result = pc(stream, tracker, collector);
            }
            return tokens;
        };
    }

    inline auto many1(ParserCombinator const& pc) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            // return (match(type) & (*type))(stream,tracker,collector);

            if (ParserCombinatorResult const result = many0(pc)(stream, tracker, collector);
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
                if (auto const result = p(stream, tracker, collector); result) {
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
                auto const result = p(stream, tracker, collector);
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
