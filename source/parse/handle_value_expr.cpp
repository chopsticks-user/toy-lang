#include "parse.hpp"

namespace tlc::parse {
    auto handlePrimaryExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return {};
        };
    }

    auto handleExpr() -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            return any(
                seq(match(token::EToken::Plus2, token::EToken::Minus2),
                    match(token::EToken::Plus2, token::EToken::Minus2))
            )(context, stream, panic);
        };
    }
}
