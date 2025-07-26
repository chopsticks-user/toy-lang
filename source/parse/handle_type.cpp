#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleType() -> ParseResult {
        pushCoords();

        auto lhs =
            handleTypeInfer().or_else(
                [this](auto const&) -> ParseResult {
                    return handleTypeTuple().or_else(
                        [this](auto const&) -> ParseResult {
                            return handleTypeIdentifier();
                        }
                    );
                }
            );
        if (!lhs) {
            return Unexpected{lhs.error()};
        }

        m_stream.markBacktrack();
        while (true) {
            if (m_stream.peek().type() == LeftBracket) {
                Vec<syntax::Node> dimSizes;
                while (m_stream.match(LeftBracket)) {
                    dimSizes.emplace_back(handleExpr().value_or(syntax::Node{}));
                    if (!m_stream.match(RightBracket)) {
                        // todo: collect errors
                        break;
                    }
                }
                lhs = syntax::type::Array{*lhs, dimSizes, currentCoords()};
            }
            else if (m_stream.match(MinusGreater)) {
                auto fnResultType = handleType()
                    .or_else([this, &lhs](auto&& Error)
                        -> ParseResult {
                            // todo: collect errors, if any
                            return syntax::Node{};
                        });
                lhs = syntax::type::Function{
                    *lhs, std::move(*fnResultType), currentCoords()
                };
            }
            else if (false) {
                // todo: binary operators on types
            }
            else {
                break;
            }
        }

        return lhs;
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
                    std::move(path),
                    m_stream.current().type() == FundamentalType,
                    tokens.front().coords()
                };
            }
        );
    }

    auto Parse::handleTypeTuple() -> ParseResult {
        return match(LeftParen)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                Vec<syntax::Node> elements;
                do {
                    if (auto const type = handleType(); type) {
                        elements.push_back(*type);
                    }
                    else {
                        // todo: error
                    }
                }
                while (m_stream.match(Comma));

                if (!m_stream.match(RightParen)) {
                    return Unexpected{Error{}};
                }
                return syntax::type::Tuple{
                    std::move(elements),
                    std::move(coords)
                };
            }
        );
    }

    auto Parse::handleTypeInfer() -> ParseResult {
        return seq(match(LeftBracket), match(LeftBracket))
            (m_context, m_stream, m_panic).and_then([this](auto const&)
                -> ParseResult {
                    return handleExpr().and_then([this](auto const& expr)
                        -> ParseResult {
                            if (!seq(match(RightBracket), match(RightBracket))
                                (m_context, m_stream, m_panic)) {
                                return Unexpected{Error{}};
                            }
                            return syntax::type::Infer{expr, currentCoords()};
                        });
                });
    }
}
