#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleType() -> ParseResult {
        return handleTypeIdentifier();
    }

    auto Parse::handleTypeIdentifier() -> ParseResult {
        return seq(
            many0(seq(match(Identifier), match(Colon2))),
            match(FundamentalType, UserDefinedType)
        )(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto path = tokens
                    | rv::take(tokens.size() - 1)
                    | rv::filter([](auto&& token) {
                        return token.type() == Identifier;
                    })
                    | rv::transform([](auto&& token) { return token.str(); })
                    | rng::to<Vec<Str>>();
                path.push_back(Str{tokens.back().str()});
                return syntax::type::Identifier{
                    std::move(path), m_stream.current().type() == FundamentalType,
                    tokens.front().coords()
                };
            }
        );
    }
}
