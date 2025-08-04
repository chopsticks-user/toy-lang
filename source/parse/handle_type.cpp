#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleType() -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();
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

        while (true) {
            if (m_stream.match(lexeme::leftBracket)) {
                // todo:
                Vec<syntax::Node> sizes;
                do {
                    sizes.push_back(*handleExpr().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error).collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::Array,
                                    .reason = EParseErrorReason::MissingExpr,
                                });
                                return {};
                            }
                        )
                    );
                }
                while (m_stream.match(lexeme::comma));

                if (!m_stream.match(lexeme::rightBracket)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Array,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                lhs = syntax::type::Array{*lhs, std::move(sizes), *location};
            }
            else if (m_stream.match(lexeme::less)) {
                auto const argsLocation = m_tracker.current();
                Vec<syntax::Node> types;
                do {
                    types.push_back(*handleType().or_else(
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

                lhs = syntax::type::Generic{
                    *lhs, syntax::type::GenericArguments{
                        std::move(types), argsLocation
                    },
                    *location
                };
            }
            else if (m_stream.match(lexeme::minusGreater)) {
                auto fnResultType = handleType()
                    .or_else([this]([[maybe_unused]] auto&& error)
                        -> ParseResult {
                            collect(error);
                            return {};
                        });
                lhs = syntax::type::Function{
                    *lhs, std::move(*fnResultType), *location
                };
            }
            else {
                break;
            }
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
            (m_stream, m_tracker).and_then([this](auto const& tokens)
                -> ParseResult {
                    return handleExpr().and_then([this, &tokens](auto const& expr)
                        -> ParseResult {
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
                        });
                }
            );
    }
}
