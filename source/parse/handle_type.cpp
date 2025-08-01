#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleType() -> ParseResult { // NOLINT(*-no-recursion)
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
            popCoords();
            collect(lhs.error());
            return Unexpected{lhs.error()};
        }

        m_stream.markBacktrack();
        while (true) {
            if (m_stream.peek().lexeme() == lexeme::leftBracket) {
                Vec<syntax::Node> dimSizes;
                while (m_stream.match(lexeme::leftBracket)) {
                    dimSizes.emplace_back(handleExpr().value_or({}));
                    if (!m_stream.match(lexeme::rightBracket)) {
                        collect({
                            .location = m_stream.current().coords(),
                            .context = EParseErrorContext::Array,
                            .reason = EParseErrorReason::MissingEnclosingSymbol,
                        });
                        break;
                    }
                }
                lhs = syntax::type::Array{*lhs, dimSizes, currentCoords()};
            }
            else if (m_stream.match(lexeme::minusGreater)) {
                auto fnResultType = handleType()
                    .or_else([this]([[maybe_unused]] auto&& error)
                        -> ParseResult {
                            collect(error);
                            return {};
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
            many0(seq(match(lexeme::identifier), match(lexeme::colon2))),
            match(lexeme::fundamentalType, lexeme::userDefinedType)
        )(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto path = tokens
                    | rv::take(tokens.size() - 1)
                    | rv::filter([](auto&& token) {
                        return token.lexeme() == lexeme::identifier;
                    })
                    | rv::transform([](auto&& token) { return token.str(); })
                    | rng::to<Vec<Str>>();
                path.push_back(Str{tokens.back().str()});
                return syntax::type::Identifier{
                    std::move(path),
                    m_stream.current().lexeme() == lexeme::fundamentalType,
                    tokens.front().coords()
                };
            }
        );
    }

    auto Parse::handleTypeTuple() -> ParseResult {
        return match(lexeme::leftParen)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                Vec<syntax::Node> types;

                if (m_stream.match(lexeme::rightParen)) {
                    return syntax::type::Tuple{
                        std::move(types), std::move(coords)
                    };
                }

                do {
                    types.push_back(*handleType().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error).collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::Tuple,
                                    .reason = EParseErrorReason::MissingType,
                                });
                                return syntax::Node{};
                            }
                        )
                    );
                }
                while (m_stream.match(lexeme::comma));

                if (!m_stream.match(lexeme::rightParen)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Tuple,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::type::Tuple{
                    std::move(types),
                    std::move(coords)
                };
            }
        );
    }

    auto Parse::handleTypeInfer() -> ParseResult {
        return seq(match(lexeme::leftBracket), match(lexeme::leftBracket))
            (m_stream, m_tracker).and_then([this](auto const&)
                -> ParseResult {
                    return handleExpr().and_then([this](auto const& expr)
                        -> ParseResult {
                            if (!seq(match(lexeme::rightBracket), match(lexeme::rightBracket))
                                (m_stream, m_tracker)) {
                                collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::TypeInfer,
                                    .reason = EParseErrorReason::MissingEnclosingSymbol,
                                });
                            }
                            return syntax::type::Infer{expr, currentCoords()};
                        });
                }
            );
    }
}
