#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleType(syntax::OpPrecedence const minP) -> ParseResult {
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();
        auto lhs = [this] {
            if (auto infer = handleTypeInfer(); infer) {
                return infer;
            }
            if (auto tuple = handleTypeTuple(); tuple) {
                return tuple;
            }
            return handleTypeIdentifier();
        }();
        if (!lhs) {
            return defaultError();
        }

        auto streamBacktrack = m_stream.scopedBacktrack();
        while (true) {
            if (auto array = handleArrayExpr(); array) {
                lhs = syntax::type::Array{
                    *lhs, std::move(*array), *location
                };
                continue;
            }
            if (auto genericArgs = handleGenericArguments(); genericArgs) {
                lhs = syntax::type::Generic{
                    *lhs, std::move(*genericArgs), *location
                };
                continue;
            }
            if (m_stream.match(lexeme::minusGreater)) {
                auto fnResultType = handleType()
                    .or_else([this]([[maybe_unused]] auto&& error)
                        -> ParseResult {
                            collect(error);
                            return {};
                        });
                lhs = syntax::type::Function{
                    *lhs, std::move(*fnResultType), *location
                };
                continue;
            }
            if (m_stream.match(syntax::isBinaryTypeOperator)) {
                auto const op = m_stream.current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );

                if (p <= minP) {
                    streamBacktrack();
                    break;
                }

                lhs = handleType(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).and_then([&](auto const& rhs) -> ParseResult {
                    return syntax::type::Binary{
                        *lhs, op, rhs, *location
                    };
                });
                continue;
            }
            break;
        }

        return lhs;
    }

    auto Parse::handleTypeIdentifier() -> ParseResult {
        TLC_SCOPE_REPORTER();
        auto const constant = !m_stream.match(lexeme::dollar);
        auto const location = m_tracker.current();
        return seq(
            many0(seq(match(lexeme::identifier), match(lexeme::dot))),
            match(lexeme::fundamentalType, lexeme::userDefinedType)
        )(m_stream, m_tracker).and_then(
            [&](auto const& tokens) -> ParseResult {
                auto path = tokens
                    | rv::take(tokens.size() - 1)
                    | rv::filter([](auto&& token) {
                        return token.lexeme() == lexeme::identifier;
                    })
                    | rv::transform([](auto&& token) { return token.str(); })
                    | rng::to<Vec<Str>>();
                path.push_back(Str{tokens.back().str()});
                return syntax::type::Identifier{
                    constant, std::move(path),
                    m_stream.current().lexeme() == lexeme::fundamentalType,
                    constant ? tokens.front().location() : location
                };
            }
        );
    }

    auto Parse::handleTypeTuple() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::leftParen)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                Vec<syntax::Node> types;

                if (m_stream.match(lexeme::rightParen)) {
                    return syntax::type::Tuple{
                        std::move(types), std::move(location)
                    };
                }

                do {
                    types.push_back(*handleType().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error).collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::Tuple,
                                    .reason = EParseErrorReason::MissingType,
                                });
                                return {};
                            }
                        )
                    );
                }
                while (m_stream.match(lexeme::comma));

                if (!m_stream.match(lexeme::rightParen)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Tuple,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::type::Tuple{std::move(types), location};
            }
        );
    }

    auto Parse::handleTypeInfer() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return seq(match(lexeme::leftBracket), match(lexeme::leftBracket))
            (m_stream, m_tracker).and_then(
                [this](auto const& tokens) -> ParseResult {
                    return handleExpr().and_then(
                        [this, &tokens](auto const& expr) -> ParseResult {
                            if (!seq(match(lexeme::rightBracket),
                                     match(lexeme::rightBracket))
                                (m_stream, m_tracker)) {
                                collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::TypeInfer,
                                    .reason = EParseErrorReason::MissingEnclosingSymbol,
                                });
                            }
                            return syntax::type::Infer{
                                expr, tokens.front().location()
                            };
                        }
                    );
                }
            );
    }

    auto Parse::handleGenericArguments() -> ParseResult {
        if (!m_stream.match(lexeme::less)) {
            return defaultError();
        }

        auto location = m_tracker.current();
        Vec<syntax::Node> args;
        do {
            args.push_back(*handleType().or_else(
                [this](auto&& error) -> ParseResult {
                    collect(error).collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::GenericTypeArguments,
                        .reason = EParseErrorReason::MissingType,
                    });
                    return {};
                }
            ));
        }
        while (m_stream.match(lexeme::comma));

        if (!m_stream.match(lexeme::greater)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::GenericTypeArguments,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }
        return syntax::type::GenericArguments{
            std::move(args), std::move(location)
        };
    }
}
