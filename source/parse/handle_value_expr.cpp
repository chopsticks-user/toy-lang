#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto handlePrimaryExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handleExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            token::TokenizedBuffer lhsNodes;
            if (stream.match(Plus2, Minus2)) {
                // todo: get op precedence
                auto [success, nodes, _] =
                    handleExpr()(context, stream, panic);
                if (!success) {
                    return {};
                }
                lhsNodes = std::move(nodes);
            }
            else {
                auto [success, nodes, _] =
                    handleExpr()(context, stream, panic);
                if (!success) {
                    return {};
                }
                lhsNodes = std::move(nodes);
            }

            while (true) {}

            return any(
                seq(handlePrimaryExpr(), match(Plus2, Minus2))
            )(context, stream, panic);
        };
    }
}
