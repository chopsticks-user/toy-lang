#ifndef TLC_PARSE_TOKEN_STREAM_HPP
#define TLC_PARSE_TOKEN_STREAM_HPP

#include "syntax/syntax.hpp"
#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::parse {
    class TokenStream final {
    public:
        explicit TokenStream(Vec<token::Token> tokens)
            : m_tokens{std::move(tokens)},
              m_tokenIt{m_tokens.begin()} {}

        auto match(std::same_as<token::EToken> auto... types) -> bool {
            if (done() || ((current().type() != types) && ...)) {
                return false;
            }

            advance();
            return true;
        }

        auto advance() -> void {
            ++m_tokenIt;
        }

        [[nodiscard]] auto peek() const -> token::Token {
            if (auto const nextIt = std::next(m_tokenIt);
                nextIt != m_tokens.end()) {
                return *nextIt;
            }
            return makeInvalidToken();
        }

        auto markCurrentToken() -> void {
            m_markedTokens.push(current());
        }

        [[nodiscard]] auto unmarkAndReturnCoords()
            -> token::Token::Coords {
            if (m_markedTokens.empty()) {
                return makeInvalidToken().coords();
            }

            auto const coords = m_markedTokens.top().coords();
            m_markedTokens.pop();
            return coords;
        }

        [[nodiscard]] auto current() const -> token::Token {
            if (done()) {
                return makeInvalidToken();
            }
            return *m_tokenIt;
        }

        [[nodiscard]] auto done() const -> bool {
            return m_tokenIt == m_tokens.end();
        }

    private:
        static auto makeInvalidToken() -> token::Token {
            return {token::EToken::Invalid, "", {0, 0}};
        }

    private:
        Vec<token::Token> const m_tokens;
        Vec<token::Token>::const_iterator m_tokenIt;
        Stack<token::Token> m_markedTokens{};
    };
}

#endif // TLC_PARSE_TOKEN_STREAM_HPP
