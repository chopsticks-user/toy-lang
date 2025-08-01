#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleExpr(syntax::OpPrecedence const minP) -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();
        ParseResult lhs;

        // todo: consider requiring parenthesis around each prefix expressions
        // to avoid ambiguity
        if (m_stream.match(syntax::isPrefixOperator)) {
            auto const op = m_stream.current().lexeme();
            lhs = handleExpr(
                    syntax::opPrecedence(
                        op, syntax::EOperator::Prefix
                    ))
                .and_then([&](auto const& node) -> ParseResult {
                    return syntax::expr::Prefix{node, op, *location};
                });
        }
        else {
            lhs = handlePrimaryExpr();
        }
        if (!lhs) {
            return Unexpected{lhs.error()};
        }

        m_stream.markBacktrack();
        while (true) {
            if (syntax::isPostfixStart(m_stream.peek().lexeme())) {
                if (m_stream.match(lexeme::dot)) {
                    auto field = [this] -> Str {
                        if (m_stream.match(lexeme::identifier)) {
                            return m_stream.current().str();
                        }
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::Access,
                            .reason = EParseErrorReason::MissingId,
                        });
                        return "";
                    }();
                    lhs = syntax::expr::Access{
                        *lhs, std::move(field), *location
                    };
                }
                else if (auto const tuple = handleTupleExpr(); tuple) {
                    lhs = syntax::expr::FnApp{
                        *lhs, *tuple, *location
                    };
                }
                else if (auto const array = handleArrayExpr(); array) {
                    lhs = syntax::expr::Subscript{
                        *lhs, *array, *location
                    };
                }
            }
            else if (m_stream.match(syntax::isBinaryOperator)) {
                auto const op = m_stream.current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );

                if (p <= minP) {
                    m_stream.backtrack();
                    break;
                }

                lhs = handleExpr(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).and_then([&](auto const& rhs) -> ParseResult {
                    return syntax::expr::Binary{
                        *lhs, rhs, op, *location
                    };
                });
            }
            // todo: ternary
            else if (m_stream.match(lexeme::qMark)) {}
            else {
                break;
            }
        }

        return lhs;
    }

    auto Parse::handlePrimaryExpr() -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        if (auto const result = handleSingleTokenLiteral(); result) {
            return result;
        }
        if (auto const result = handleIdentifierLiteral(); result) {
            return result;
        }
        if (auto const result = handleTupleExpr(); result) {
            return result;
        }
        if (auto const result = handleArrayExpr(); result) {
            return result;
        }
        if (auto const result = handleRecordExpr(); result) {
            return result;
        }
        return defaultError();
    }

    auto Parse::handleSingleTokenLiteral() -> ParseResult {
        TLC_SCOPE_REPORTER();
        static const HashMap<lexeme::Lexeme, i32> baseTable = {
            {lexeme::integer2Literal, 2}, {lexeme::integer8Literal, 8},
            {lexeme::integer10Literal, 10}, {lexeme::integer16Literal, 16},
        };

        return match(
            lexeme::integer2Literal, lexeme::integer8Literal,
            lexeme::integer10Literal, lexeme::integer16Literal,
            lexeme::floatLiteral, lexeme::true_, lexeme::false_
        )(m_stream, m_tracker).and_then(
            [this](const auto& tokens)
            -> ParseResult {
                auto location = tokens.front().location();
                switch (tokens.front().lexeme().type()) {
                case lexeme::Lexeme::FloatLiteral:
                    return syntax::expr::Float{
                        std::stod(tokens.front().str()), location
                    };
                case lexeme::Lexeme::True:
                case lexeme::Lexeme::False:
                    return syntax::expr::Boolean{
                        tokens.front().lexeme() == lexeme::true_, location
                    };
                default:
                    return syntax::expr::Integer{
                        std::stoll(
                            tokens.front().str(), nullptr,
                            baseTable.at(tokens.front().lexeme())
                        ),
                        location
                    };
                }
            }
        );
    }

    auto Parse::handleIdentifierLiteral() -> ParseResult {
        TLC_SCOPE_REPORTER();
        if (m_stream.match(lexeme::anonymousIdentifier)) {
            return syntax::expr::Identifier{
                {m_stream.current().str()}, m_stream.current().location()
            };
        }
        return seq(
            many0(seq(match(lexeme::identifier), match(lexeme::colon2))),
            match(lexeme::identifier)
        )(m_stream, m_tracker).and_then([this](auto const& tokens)
            -> ParseResult {
                auto path = tokens
                    | rv::take(tokens.size() - 1)
                    | rv::filter([](auto&& token) {
                        return token.lexeme() == lexeme::identifier;
                    })
                    | rv::transform([](auto&& token) { return token.str(); })
                    | rng::to<Vec<Str>>();
                path.push_back(Str{tokens.back().str()});
                return syntax::expr::Identifier{
                    std::move(path), tokens.front().location()
                };
            }
        );
    }

    auto Parse::handleTupleExpr() -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();
        if (!m_stream.match(lexeme::leftParen)) {
            return defaultError();
        }
        if (m_stream.match(lexeme::rightParen)) {
            return syntax::expr::Tuple{{}, *location};
        }

        Vec<syntax::Node> elements;
        do {
            /**
             * (x,y,z,) -> error, the ',' after 'z' expects another expr
             * (,x,y,z) -> error, the ',' before 'x' expects another expr
             * (,x,y,z,) -> error
             * (Int,x,y) -> error, "Int" is not an expr
             *
             */
            elements.push_back(*handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error).collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::Tuple,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return syntax::Node{};
                    })
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

        return syntax::expr::Tuple{std::move(elements), *location};
    }

    auto Parse::handleArrayExpr() -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();
        if (!m_stream.match(lexeme::leftBracket)) {
            return defaultError();
        }
        if (m_stream.match(lexeme::rightBracket)) {
            return syntax::expr::Array{{}, *location};
        }

        Vec<syntax::Node> elements;
        do {
            /**
             * [x,y,z,] -> error, the ',' after 'z' expects another expr
             * [,x,y,z] -> error, the ',' before 'x' expects another expr
             * [,x,y,z,] -> error
             * [Int,x,y] -> error, "Int" is not an expr
             *
             */
            elements.push_back(*handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::Array,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return syntax::Node{};
                    })
            );
        }
        while (m_stream.match(lexeme::comma));

        if (!m_stream.match(lexeme::rightBracket)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::Tuple,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Array{std::move(elements), *location};
    }

    auto Parse::handleRecordExpr() -> ParseResult { // NOLINT(*-no-recursion)
        TLC_SCOPE_REPORTER();
        m_stream.markBacktrack();
        auto const location = m_tracker.scopedLocation();

        auto type = handleTypeIdentifier();
        if (!type || !m_stream.match(lexeme::leftBrace)) {
            m_stream.backtrack();
            return defaultError();
        }

        Vec<Pair<Str, syntax::Node>> entries;

        if (m_stream.match(lexeme::rightBrace)) {
            return syntax::expr::Record{
                std::move(*type), std::move(entries), *location
            };
        }

        do {
            Str key;
            if (!m_stream.match(lexeme::identifier)) {
                collect({
                    .location = m_tracker.current(),
                    .context = EParseErrorContext::Record,
                    .reason = EParseErrorReason::MissingId,
                });
            }
            else {
                key = m_stream.current().str();
            }

            if (!m_stream.match(lexeme::colon)) {
                collect({
                    .location = m_tracker.current(),
                    .context = EParseErrorContext::Record,
                    .reason = EParseErrorReason::MissingSymbol,
                });
            }

            syntax::Node value = *handleExpr().or_else(
                [this](auto&& error) -> ParseResult {
                    collect(error).collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Record,
                        .reason = EParseErrorReason::MissingExpr,
                    });
                    return {};
                }
            );

            entries.emplace_back(std::move(key), std::move(value));
        }
        while (m_stream.match(lexeme::comma));

        if (!m_stream.match(lexeme::rightBrace)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::Record,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Record{
            std::move(*type), std::move(entries), *location
        };
    }
}
