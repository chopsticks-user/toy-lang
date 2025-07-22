#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto handlePrimaryExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return match(
                Identifier, Integer2Literal, Integer8Literal, Integer10Literal,
                Integer16Literal
            )(context, stream, panic);
        };
    }

    auto handlePostfixExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handlePrefixExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handleBinaryExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handleTernaryExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handleExpr(syntax::OpPrecedence const minPrecedence) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
            // Vec<syntax::Node> lhsNodes;
            // if (stream.match(syntax::isPrefixOperator)) {
            //     // todo: get op precedence
            //     lhsNodes = {syntax::TokenWrapper{stream.current()}};
            //     auto const precedence = syntax::opPrecedence(
            //         stream.current().type(), syntax::EOperator::Prefix
            //     );
            //     auto [success, nodes, _] =
            //         handleExpr(precedence)(context, stream, panic);
            //     if (!success) {
            //         return {};
            //     }
            //     lhsNodes.append_range(nodes);
            // }
            // else {
            //     auto [success, nodes, _] =
            //         handlePrimaryExpr()(context, stream, panic);
            //     if (!success) {
            //         return {};
            //     }
            //     lhsNodes = std::move(nodes);
            // }
            //
            // stream.markBacktrack();
            // while (true) {
            //     if (stream.match(syntax::isBinaryOperator)) {
            //         auto const precedence = syntax::opPrecedence(
            //             stream.current().type(), syntax::EOperator::Binary
            //         );
            //         if (precedence <= minPrecedence) {
            //             stream.backtrack();
            //             break;
            //         }
            //         auto rhsNodes = handleExpr(
            //             syntax::isLeftAssociative(stream.current().type())
            //                 ? precedence + 1
            //                 : precedence
            //         )(context, stream, panic);
            //         // todo: combine lhs and rhs
            //     }
            //     else if (stream.match(QMark)) {}
            //     else {
            //         break;
            //     }
            // }
            //
            // return {true, lhsNodes};
        };
    }
}
