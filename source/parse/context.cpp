#include "context.hpp"

namespace tlc::parse {
    static auto createLiteral1Node(token::TokenizedBuffer const& tokens)
        -> syntax::Node {
        switch (tokens.front().type()) {
        case token::EToken::FloatLiteral:
            return syntax::expr::Float{
                std::stod(tokens.front().str()), tokens.front().coords()
            };
        case token::EToken::True:
        case token::EToken::False:
            return syntax::expr::Boolean{
                tokens.front().type() == token::EToken::True, tokens.front().coords()
            };
        default:
            return syntax::expr::Integer{
                std::stoll(tokens.front().str()), tokens.front().coords()
            };
        }
    }

    static auto createNamespaceIdNode(token::TokenizedBuffer const& tokens)
        -> syntax::Node {
        auto path = tokens
            | rv::take(tokens.size() - 1)
            | rv::filter([](auto token) {
                return token.type() == token::EToken::Identifier;
            })
            | rv::transform([](auto token) { return token.str(); })
            | rng::to<Vec<Str>>();
        path.push_back(Str{tokens.back().str()});

        switch (tokens.back().type()) {
        case token::EToken::Identifier:
            return syntax::expr::VarId{std::move(path), tokens.front().coords()};
        default: // FundamentalType || UserDefinedType
            return syntax::expr::TypeId{std::move(path), tokens.front().coords()};
        }
    }

    auto Context::createNode(State const& state) -> syntax::Node {
        switch (state.type) {
        case EState::Literal1:
            return createLiteral1Node(state.tokens);
        case EState::NamespaceId:
            return createNamespaceIdNode(state.tokens);
        default:
            return {};
        }
    }
}
