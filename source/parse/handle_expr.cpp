#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleExpr(syntax::OpPrecedence const minP) -> ParseResult { // NOLINT(*-no-recursion)
        pushCoords();
        ParseResult lhs;

        // todo: consider requiring parenthesis around each prefix expressions
        // to avoid ambiguity
        if (m_stream.match(syntax::isPrefixOperator)) {
            auto const op = m_stream.current().lexeme();
            auto const coords =
                m_stream.current().coords();
            lhs = handleExpr(
                    syntax::opPrecedence(
                        op, syntax::EOperator::Prefix
                    ))
                .and_then([this, coords, op](auto const& node) -> ParseResult {
                    return syntax::expr::Prefix{node, op, coords};
                });
        }
        else {
            lhs = handlePrimaryExpr();
        }
        if (!lhs) {
            popCoords();
            collect(lhs.error());
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
                            .location = currentCoords(),
                            .context = EParseErrorContext::Access,
                            .reason = EParseErrorReason::MissingId,
                        });
                        return "";
                    }();
                    lhs = syntax::expr::Access{
                        *lhs, std::move(field), currentCoords()
                    };
                }
                else if (auto const tuple = handleTupleExpr(); tuple) {
                    lhs = syntax::expr::FnApp{
                        *lhs, *tuple, currentCoords()
                    };
                }
                else if (auto const array = handleArrayExpr(); array) {
                    lhs = syntax::expr::Subscript{
                        *lhs, *array, currentCoords()
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
                    popCoords();
                    break;
                }

                lhs = handleExpr(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).and_then([this, lhs, op](auto const& rhs) -> ParseResult {
                    return syntax::expr::Binary{
                        *lhs, rhs, op, currentCoords()
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
        pushCoords();
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
        popCoords();
        return defaultError();
    }

    auto Parse::handleSingleTokenLiteral() -> ParseResult {
        static const HashMap<lexeme::Lexeme, i32> baseTable = {
            {lexeme::integer2Literal, 2}, {lexeme::integer8Literal, 8},
            {lexeme::integer10Literal, 10}, {lexeme::integer16Literal, 16},
        };

        return match(
            lexeme::integer2Literal, lexeme::integer8Literal,
            lexeme::integer10Literal, lexeme::integer16Literal,
            lexeme::floatLiteral, lexeme::true_, lexeme::false_
        )(m_stream, m_tracker).and_then([this](const auto& tokens)
            -> ParseResult {
                switch (tokens.front().lexeme().type()) {
                case lexeme::Lexeme::FloatLiteral:
                    return syntax::expr::Float{
                        std::stod(tokens.front().str()),
                        tokens.front().coords()
                    };
                case lexeme::Lexeme::True:
                case lexeme::Lexeme::False:
                    return syntax::expr::Boolean{
                        tokens.front().lexeme() == lexeme::true_,
                        tokens.front().coords()
                    };
                default:
                    return syntax::expr::Integer{
                        std::stoll(
                            tokens.front().str(), nullptr,
                            baseTable.at(tokens.front().lexeme())
                        ),
                        tokens.front().coords()
                    };
                }
            }
        );
    }

    auto Parse::handleIdentifierLiteral() -> ParseResult {
        if (m_stream.match(lexeme::anonymousIdentifier)) {
            return syntax::expr::Identifier{
                {m_stream.current().str()}, m_stream.current().coords()
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
                    std::move(path), tokens.front().coords()
                };
            }
        );
    }

    auto Parse::handleTupleExpr() -> ParseResult { // NOLINT(*-no-recursion)
        if (!m_stream.match(lexeme::leftParen)) {
            return defaultError();
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;

        // ()
        if (m_stream.match(lexeme::rightParen)) {
            return syntax::expr::Tuple{std::move(elements), std::move(coords)};
        }

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
                            .location = m_stream.current().coords(),
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
                .location = m_stream.current().coords(),
                .context = EParseErrorContext::Tuple,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Tuple{std::move(elements), std::move(coords)};
    }

    auto Parse::handleArrayExpr() -> ParseResult { // NOLINT(*-no-recursion)
        if (!m_stream.match(lexeme::leftBracket)) {
            return defaultError();
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;

        // []
        if (m_stream.match(lexeme::rightBracket)) {
            return syntax::expr::Array{std::move(elements), std::move(coords)};
        }

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
                            .location = m_stream.current().coords(),
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
                .location = m_stream.current().coords(),
                .context = EParseErrorContext::Tuple,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Array{std::move(elements), std::move(coords)};
    }

    auto Parse::handleRecordExpr() -> ParseResult { // NOLINT(*-no-recursion)
        m_stream.markBacktrack();
        auto coords = m_stream.peek().coords();

        auto type = handleTypeIdentifier();
        if (!type || !m_stream.match(lexeme::leftBrace)) {
            m_stream.backtrack();
            return defaultError();
        }

        Vec<Pair<Str, syntax::Node>> entries;

        if (m_stream.match(lexeme::rightBrace)) {
            return syntax::expr::Record{
                std::move(*type), std::move(entries), std::move(coords)
            };
        }

        do {
            Str key;
            if (!m_stream.match(lexeme::identifier)) {
                collect({
                    .location = m_stream.current().coords(),
                    .context = EParseErrorContext::Record,
                    .reason = EParseErrorReason::MissingId,
                });
            }
            else {
                key = m_stream.current().str();
            }

            if (!m_stream.match(lexeme::colon)) {
                collect({
                    .location = m_stream.current().coords(),
                    .context = EParseErrorContext::Record,
                    .reason = EParseErrorReason::MissingSymbol,
                });
            }

            syntax::Node value = *handleExpr().or_else(
                [this](auto&& error) -> ParseResult {
                    collect(error).collect({
                        .location = m_stream.current().coords(),
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
                .location = coords,
                .context = EParseErrorContext::Record,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Record{
            std::move(*type), std::move(entries), std::move(coords)
        };
    }
}
