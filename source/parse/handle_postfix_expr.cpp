#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parser::handlePostfixExpr() -> ParseResult {
        return handlePrimaryExpr().and_then([this](auto&& node) -> ParseResult {
            if (m_stream.match(Dot)) {
                if (m_stream.match(Identifier, FundamentalType, UserDefinedType)) {
                    return syntax::expr::Access{
                        node, syntax::expr::Identifier{
                            {m_stream.current().str()},
                            m_stream.current().type(),
                            m_stream.current().coords()
                        },
                        currentCoords()
                    };
                }
                // todo: ignore "." and append errors
                return node;
            }


            if (auto const result = handleTupleExpr(); result) {
                return syntax::expr::FnApp{
                    node, *result, currentCoords()
                };
            }

            if (auto const result = handleArrayExpr(); result) {
                return syntax::expr::Subscript{
                    node, *result, currentCoords()
                };
            }

            return node;
        });
    }
}
