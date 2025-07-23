#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parser::handleExpr(syntax::OpPrecedence const minP) -> ParseResult {
        syntax::Node lhs;
        pushCoords();

        if (m_stream.match(syntax::isPrefixOperator)) {
            auto const op = m_stream.current().type();
            auto const coords = m_stream.current().coords();
            if (auto const result = handleExpr(
                syntax::opPrecedence(
                    op, syntax::EOperator::Prefix
                )); result) {
                lhs = syntax::expr::Unary{*result, op, coords};
            }
            else {
                // todo: error
                return Unexpected{result.error()};
            }
        }
        else if (auto const result = handlePostfixExpr(); result) {
            lhs = *result;
            popCoords();
        }
        else {
            // todo: error
            return Unexpected{result.error()};
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

                if (auto const result = handleExpr(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).and_then([this, &lhs, op](auto const& rhs) -> ParseResult {
                    return syntax::expr::Binary{
                        std::move(lhs), rhs, op, currentCoords()
                    };
                }); result) {
                    lhs = *result;
                }
                else {
                    return Unexpected{result.error()};
                }
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
