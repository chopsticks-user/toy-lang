#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    struct ParserCombinatorError {};

    // struct ParserCombinatorResult {
    //     bool success = false;
    //     Vec<syntax::Node> nodes;
    //     Opt<szt> rule;
    // };

    using ParserCombinatorResult = Expected<
        token::TokenizedBuffer, ParserCombinatorError
    >;

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
                return Unexpected{ParserCombinatorError{}};
            }
            return {{stream.current()}};
        };
    }

    inline auto match(TokenStream::MatchFn const cond) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(cond)) {
                return Unexpected{ParserCombinatorError{}};
            }
            return {{stream.current()}};
        };
    }

    auto many0(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer tokens;
            while (stream.match(types...)) {
                tokens.emplace_back(stream.current());
            }
            return tokens;
        };
    }

    auto many1(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            // return (match(type) & (*type))(context, stream, panic);
            ParserCombinatorResult result = many0(types...)(context, stream, panic);
            return result.and_then(
                [](auto const& tokens) {
                    return tokens.empty()
                               ? Unexpected{ParserCombinatorError{}}
                               : tokens;
                }
            );
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
            return Unexpected{ParserCombinatorError{}};
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
                    return Unexpected{ParserCombinatorError{}};
                }
                tokens.append_range(result.value());
            }

            return tokens;
        };
    }

    auto handlePrimaryExpr() -> ParserCombinator;
    auto handlePostfixExpr() -> ParserCombinator;
    auto handlePrefixExpr() -> ParserCombinator;
    auto handleBinaryExpr() -> ParserCombinator;
    auto handleTernaryExpr() -> ParserCombinator;
    auto handleExpr(syntax::OpPrecedence minPrecedence = 0) -> ParserCombinator;
}

#endif // TLC_PARSE_COMBINATOR_HPP
