#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    struct ParserCombinatorResult {
        bool success = false;
        token::TokenizedBuffer nodes;
        Opt<szt> rule;
    };

    using ParserCombinator = std::function<
        ParserCombinatorResult(
            Context& context, TokenStream& stream, Panic& panic
        )
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [=]([[maybe_unused]] Context& context, [[maybe_unused]] TokenStream& stream, [[maybe_unused]] Panic& panic) \
        -> ParserCombinatorResult

    auto match(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(types...)) {
                return {};
            }
            return {true, {stream.current()}};
        };
    }

    inline auto match(TokenStream::MatchFn const cond) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(cond)) {
                return {};
            }
            return {true, {stream.current()}};
        };
    }

    auto many0(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            ParserCombinatorResult result{true, {}};
            while (stream.match(types...)) {
                result.nodes.emplace_back(stream.current());
            }
            return result;
        };
    }

    auto many1(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            // return (match(type) & (*type))(context, stream, panic);
            auto result = many0(types...)(context, stream, panic);
            if (result.nodes.empty()) {
                result.success = false;
            }
            return result;
        };
    }

    auto any(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            for (szt rule = 0; auto&& p : {pc...}) {
                if (auto result = p(context, stream, panic);
                    result.success) {
                    result.rule = rule;
                    return result;
                }
                ++rule;
            }
            return {};
        };
    }

    auto seq(
        std::same_as<ParserCombinator> auto&&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            ParserCombinatorResult result{true, {}};
            for (szt rule = 0; auto&& p : {pc...}) {
                const auto [success, nodes, _] =
                    p(context, stream, panic);
                if (!success) {
                    // todo: revert to the original state
                    return {};
                }
                result.nodes.append_range(nodes);
            }

            return result;
        };
    }
}

#endif // TLC_PARSE_COMBINATOR_HPP
