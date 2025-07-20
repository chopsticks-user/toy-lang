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
            return any(
                seq(handlePrimaryExpr(), match(Plus2, Minus2))
            )(context, stream, panic);
        };
    }
}
