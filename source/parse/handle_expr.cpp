#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parser::handleExpr(syntax::OpPrecedence const minP) -> ParseResult {
        syntax::Node lhs;
        if (auto const result = handlePrefixExpr(); !result) {
            return Unexpected{Error{}};
        }
        else {
            lhs = std::move(*result);
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
                ).and_then([this, &lhs, op](auto&& rhs) -> ParseResult {
                    return syntax::expr::Binary{
                        std::move(lhs), std::move(rhs), op, currentCoords()
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

    auto Parser::handleTernaryExpr(syntax::OpPrecedence minP) -> ParseResult {}

    auto Parser::handleBinaryExpr(syntax::OpPrecedence minP) -> ParseResult {}

    auto Parser::handlePrefixExpr() -> ParseResult {
        pushCoords();
        if (!m_stream.match(syntax::isPrefixOperator)) {
            popCoords();
            return handlePostfixExpr();
        }

        return handlePostfixExpr().and_then([this](auto&& node) -> ParseResult {
            return syntax::expr::Unary{
                node, m_stream.current().type(), currentCoords()
            };
        });
    }

    // auto handleExpr(syntax::OpPrecedence const minPrecedence) -> ParserCombinator {
    //     return TLC_PARSER_COMBINATOR_PROTOTYPE {
    //         return {};
    //         // Vec<syntax::Node> lhsNodes;
    //         // if (stream.match(syntax::isPrefixOperator)) {
    //         //     // todo: get op precedence
    //         //     lhsNodes = {syntax::TokenWrapper{stream.current()}};
    //         //     auto const precedence = syntax::opPrecedence(
    //         //         stream.current().type(), syntax::EOperator::Prefix
    //         //     );
    //         //     auto [success, nodes, _] =
    //         //         handleExpr(precedence)(context, stream, panic);
    //         //     if (!success) {
    //         //         return {};
    //         //     }
    //         //     lhsNodes.append_range(nodes);
    //         // }
    //         // else {
    //         //     auto [success, nodes, _] =
    //         //         handlePrimaryExpr()(context, stream, panic);
    //         //     if (!success) {
    //         //         return {};
    //         //     }
    //         //     lhsNodes = std::move(nodes);
    //         // }
    //         //
    //         // stream.markBacktrack();
    //         // while (true) {
    //         //     if (stream.match(syntax::isBinaryOperator)) {
    //         //         auto const precedence = syntax::opPrecedence(
    //         //             stream.current().type(), syntax::EOperator::Binary
    //         //         );
    //         //         if (precedence <= minPrecedence) {
    //         //             stream.backtrack();
    //         //             break;
    //         //         }
    //         //         auto rhsNodes = handleExpr(
    //         //             syntax::isLeftAssociative(stream.current().type())
    //         //                 ? precedence + 1
    //         //                 : precedence
    //         //         )(context, stream, panic);
    //         //         // todo: combine lhs and rhs
    //         //     }
    //         //     else if (stream.match(QMark)) {}
    //         //     else {
    //         //         break;
    //         //     }
    //         // }
    //         //
    //         // return {true, lhsNodes};
    //     };
    // }
}
