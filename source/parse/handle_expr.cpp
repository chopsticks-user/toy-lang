#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parser::handleExpr(syntax::OpPrecedence const minP) -> ParseResult {
        ParseResult lhs;
        pushCoords();

        if (m_stream.match(syntax::isPrefixOperator)) {
            auto const op = m_stream.current().type();
            auto const coords =
                m_stream.current().coords();
            lhs = handleExpr(
                    syntax::opPrecedence(
                        op, syntax::EOperator::Prefix
                    ))
                .and_then([this, coords, op](auto const& node) -> ParseResult {
                    return syntax::expr::Unary{node, op, coords};
                });
        }
        else {
            lhs = handlePostfixExpr();
            popCoords();
        }

        if (!lhs) {
            return Unexpected{Error{}};
        }

        m_stream.markBacktrack();
        while (true) {
            if (m_stream.match(syntax::isBinaryOperator)) {
                auto const op = m_stream.current().type();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );

                if (p <= minP) {
                    m_stream.backtrack();
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
}
