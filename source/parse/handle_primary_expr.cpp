#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

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
            std::ignore = handleExpr().transform([this, &elements](auto&& node) {
                elements.emplace_back(node);
            });
        }
        while (match(Comma));

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
            std::ignore = handleExpr().transform([this, &elements](auto&& node) {
                elements.emplace_back(node);
            });
        }
        while (match(Comma));

        if (!m_stream.match(RightBracket)) {
            return Unexpected{Error{}};
        }

        return syntax::expr::Tuple{std::move(elements), std::move(coords)};
    }
}
