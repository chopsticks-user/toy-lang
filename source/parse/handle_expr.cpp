#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parser::handleExpr(syntax::OpPrecedence const minP) -> ParseResult {
        ParseResult lhs;
        pushCoords();

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
            return Unexpected{Error{}};
        }

        m_stream.markBacktrack();
        while (true) {
            if (syntax::isPostfixStart(m_stream.peek().type())) {
                // todo: current token might be eof
                if (m_stream.match(Dot)) {
                    if (m_stream.match(Identifier, FundamentalType, UserDefinedType)) {
                        lhs = syntax::expr::Access{
                            *lhs, syntax::expr::Identifier{
                                {m_stream.current().str()},
                                m_stream.current().type(),
                                m_stream.current().coords()
                            },
                            currentCoords()
                        };
                    }
                    else {
                        // todo: ignore "." and append errors
                    }
                }
                else if (auto const tuple = handleTupleExpr(); tuple) {
                    lhs = syntax::expr::FnApp{
                        *lhs, *tuple, currentCoords()
                    };
                }
                else if (auto const array = handleArrayExpr(); array) {
                    // todo: error if array is empty
                    lhs = syntax::expr::Subscript{
                        *lhs, *array, currentCoords()
                    };
                }
                else {
                    // todo: error
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

    auto Parser::handlePrimaryExpr() -> ParseResult {
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
        return Unexpected{Error{}};
    }

    auto Parser::handleSingleTokenLiteral() -> ParseResult {
        static const HashMap<token::EToken, i32> baseTable = {
            {Integer2Literal, 2}, {Integer8Literal, 8},
            {Integer10Literal, 10}, {Integer16Literal, 16},
        };
        if (auto const result = match(
            Integer2Literal, Integer8Literal, Integer10Literal, Integer16Literal,
            FloatLiteral, True, False
        )(m_context, m_stream, m_panic); result) {
            switch (auto const& tokens = result.value();
                tokens.front().type()) {
            case FloatLiteral:
                return syntax::expr::Float{
                    std::stod(tokens.front().str()), tokens.front().coords()
                };
            case True:
            case False:
                return syntax::expr::Boolean{
                    tokens.front().type() == True, tokens.front().coords()
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

        return Unexpected{Error{}};
    }

    auto Parser::handleIdentifierLiteral() -> ParseResult {
        if (auto const result = seq(
            many0(seq(match(Identifier), match(Colon2))),
            match(Identifier, FundamentalType, UserDefinedType)
        )(m_context, m_stream, m_panic); result) {
            auto const& tokens = result.value();

            auto path = tokens
                | rv::take(tokens.size() - 1)
                | rv::filter([](auto&& token) {
                    return token.type() == Identifier;
                })
                | rv::transform([](auto&& token) { return token.str(); })
                | rng::to<Vec<Str>>();
            path.push_back(Str{tokens.back().str()});
            return syntax::expr::Identifier{
                std::move(path), m_stream.current().type(), tokens.front().coords()
            };
        }

        return Unexpected{Error{}};
    }

    auto Parser::handleTupleExpr() -> ParseResult {
        if (!m_stream.match(LeftParen)) {
            return Unexpected{Error{}};
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;
        do {
            if (auto const expr = handleExpr(); expr) {
                elements.push_back(*expr);
            }
            else {
                // todo: error
            }
        }
        while (m_stream.match(Comma));

        if (!m_stream.match(RightParen)) {
            return Unexpected{Error{}};
        }

        return syntax::expr::Tuple{std::move(elements), std::move(coords)};
    }

    auto Parser::handleArrayExpr() -> ParseResult {
        if (!m_stream.match(LeftBracket)) {
            return Unexpected{Error{}};
        }

        auto coords = m_stream.current().coords();
        Vec<syntax::Node> elements;
        do {
            if (auto const expr = handleExpr(); expr) {
                elements.push_back(*expr);
            }
            else {
                // todo: error
            }
        }
        while (m_stream.match(Comma));

        if (!m_stream.match(RightBracket)) {
            return Unexpected{Error{}};
        }

        return syntax::expr::Array{std::move(elements), std::move(coords)};
    }
}
