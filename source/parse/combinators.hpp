#ifndef TLC_PARSE_COMBINATORS_HPP
#define TLC_PARSE_COMBINATORS_HPP

#include "context.hpp"
#include "panic.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    struct ParserCombinatorResult {
        bool success;
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

    auto operator~(token::EToken type) -> ParserCombinator;

    auto zeroOrMany(std::same_as<token::EToken> auto... types)
        -> ParserCombinator;

    auto operator*(token::EToken type) -> ParserCombinator;

    auto oneOrMany(std::same_as<token::EToken> auto... types)
        -> ParserCombinator;

    auto operator+(token::EToken type) -> ParserCombinator;

    auto operator|(ParserCombinator const& pc1, ParserCombinator const& pc2)
        -> ParserCombinator;

    auto operator&(ParserCombinator const& pc1, ParserCombinator const& pc2)
        -> ParserCombinator;
}

#endif // TLC_PARSE_COMBINATORS_HPP
