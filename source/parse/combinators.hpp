#ifndef TLC_PARSE_COMBINATORS_HPP
#define TLC_PARSE_COMBINATORS_HPP

#include "core/core.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    struct ParserCombinatorResult {
        bool success = false;
        Vec<syntax::Node> nodes;
    };

    using ParserCombinator = std::function<
        ParserCombinatorResult(
            Context& context, TokenStream& stream, Panic& panic
        )
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [&]([[maybe_unused]] Context& context, [[maybe_unused]] TokenStream& stream, [[maybe_unused]] Panic& panic) \
        -> ParserCombinatorResult

    auto match(std::same_as<token::EToken> auto... types)
        -> ParserCombinator;

    auto zeroOrMany(std::same_as<token::EToken> auto... types)
        -> ParserCombinator;

    auto oneOrMany(std::same_as<token::EToken> auto... types)
        -> ParserCombinator;

    auto pcOr(std::same_as<ParserCombinator> auto const&... pc)
        -> ParserCombinator;

    auto pcAnd(std::same_as<ParserCombinator> auto const&... pc)
        -> ParserCombinator;
}

#endif // TLC_PARSE_COMBINATORS_HPP
