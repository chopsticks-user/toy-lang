#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleExpr(syntax::OpPrecedence const minP) -> ParseResult { // NOLINT(*-no-recursion)
        pushCoords();
        ParseResult lhs;

        // todo: consider requiring parenthesis around each prefix expressions
        // to avoid ambiguity
        if (m_stream.match(syntax::isPrefixOperator)) {
            auto const op = m_stream.current().type();
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
            m_panic.collect(lhs.error());
            return Unexpected{lhs.error()};
        }

        m_stream.markBacktrack();
        while (true) {
            if (syntax::isPostfixStart(m_stream.peek().type())) {
                if (m_stream.match(Dot)) {
                    pushCoords();
                    auto field = *handleIdentifierLiteral()
                        .or_else(
                            [this](auto const&) -> ParseResult {
                                m_panic.collect({
                                    .location = currentCoords(),
                                    .context = Error::Context::Access,
                                    .reason = Error::Reason::MissingId,
                                });
                                return syntax::expr::Identifier{
                                    {}, currentCoords()
                                };
                            }
                        );
                    lhs = syntax::expr::Access{
                        *lhs, std::move(field), currentCoords()
                    };
                    popCoords();
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
                auto const op = m_stream.current().type();
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
            else if (m_stream.match(QMark)) {}
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
        popCoords();
        return defaultError();
    }

    auto Parse::handleSingleTokenLiteral() -> ParseResult {
        static const HashMap<token::EToken, i32> baseTable = {
            {Integer2Literal, 2}, {Integer8Literal, 8},
            {Integer10Literal, 10}, {Integer16Literal, 16},
        };

        return match(
            Integer2Literal, Integer8Literal, Integer10Literal, Integer16Literal,
            FloatLiteral, True, False
        )(m_context, m_stream, m_panic).and_then([this](const auto& tokens)
            -> ParseResult {
                switch (tokens.front().type()) {
                case FloatLiteral:
                    return syntax::expr::Float{
                        std::stod(tokens.front().str()),
                        tokens.front().coords()
                    };
                case True:
                case False:
                    return syntax::expr::Boolean{
                        tokens.front().type() == True,
                        tokens.front().coords()
                    };
                default:
                    return syntax::expr::Integer{
                        std::stoll(
                            tokens.front().str(), nullptr,
                            baseTable.at(tokens.front().type())
                        ),
                        tokens.front().coords()
                    };
                }
            }
        );
    }

    auto Parse::handleIdentifierLiteral() -> ParseResult {
        return seq(
            many0(seq(match(Identifier), match(Colon2))),
            match(Identifier)
        )(m_context, m_stream, m_panic).and_then([this](auto const& tokens)
            -> ParseResult {
                auto path = tokens
                    | rv::take(tokens.size() - 1)
                    | rv::filter([](auto&& token) {
                        return token.type() == Identifier;
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
        if (!m_stream.match(LeftParen)) {
            return defaultError();
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;

        // ()
        if (m_stream.match(RightParen)) {
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
                        m_panic.collect(error);
                        m_panic.collect({
                            .location = m_stream.current().coords(),
                            .context = Error::Context::Tuple,
                            .reason = Error::Reason::MissingExpr,
                        });
                        return syntax::Node{};
                    })
            );
        }
        while (m_stream.match(Comma));

        if (!m_stream.match(RightParen)) {
            m_panic.collect({
                .location = m_stream.current().coords(),
                .context = Error::Context::Tuple,
                .reason = Error::Reason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Tuple{std::move(elements), std::move(coords)};
    }

    auto Parse::handleArrayExpr() -> ParseResult { // NOLINT(*-no-recursion)
        if (!m_stream.match(LeftBracket)) {
            return defaultError();
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;

        // []
        if (m_stream.match(RightBracket)) {
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
                        m_panic.collect(error);
                        m_panic.collect({
                            .location = m_stream.current().coords(),
                            .context = Error::Context::Array,
                            .reason = Error::Reason::MissingExpr,
                        });
                        return syntax::Node{};
                    })
            );
        }
        while (m_stream.match(Comma));

        if (!m_stream.match(RightBracket)) {
            m_panic.collect({
                .location = m_stream.current().coords(),
                .context = Error::Context::Tuple,
                .reason = Error::Reason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Array{std::move(elements), std::move(coords)};
    }

    auto Parse::handleRecordExpr() -> ParseResult {
        m_stream.markBacktrack();

        auto type = handleTypeIdentifier().value_or({});
        if (!m_stream.match(LeftBrace)) {
            m_stream.backtrack();
            return defaultError();
        }

        auto coords = m_stream.current().coords();
        Vec<Pair<Str, syntax::Node>> entries;

        if (!m_stream.match(RightBrace)) {
            return syntax::expr::Record{
                std::move(type), std::move(entries), std::move(coords)
            };
        }

        do {
            Str key;
            if (!m_stream.match(Identifier)) {
                m_panic.collect({
                    .location = m_stream.current().coords(),
                    .context = Error::Context::Record,
                    .reason = Error::Reason::MissingId,
                });
            }
            else {
                key = m_stream.current().str();
            }

            if (!m_stream.match(Colon)) {
                m_panic.collect({
                    .location = m_stream.current().coords(),
                    .context = Error::Context::Record,
                    .reason = Error::Reason::MissingSymbol,
                });
            }

            syntax::Node value = *handleExpr().or_else(
                [this](auto&& error) -> ParseResult {
                    m_panic.collect(error);
                    m_panic.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Record,
                        .reason = Error::Reason::MissingExpr,
                    });
                    return {};
                }
            );

            entries.emplace_back(std::move(key), std::move(value));
        }
        while (m_stream.match(Comma));

        if (!m_stream.match(RightBrace)) {
            m_panic.collect({
                .location = coords,
                .context = Error::Context::Record,
                .reason = Error::Reason::MissingEnclosingSymbol,
            });
        }

        return syntax::expr::Record{
            std::move(type), std::move(entries), std::move(coords)
        };
    }
}
