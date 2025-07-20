#ifndef TLC_PARSE_STREAM_HPP
#define TLC_PARSE_STREAM_HPP

#include "syntax/syntax.hpp"
#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::parse {
    class Stream final {
    public:
        explicit Stream(token::TokenizedBuffer tokens)
            : m_tokens{std::move(tokens)},
              m_tokenIt{m_tokens.begin()} {}

        auto match(std::same_as<token::EToken> auto... types) -> bool {
            auto const tokenType = peek().type();
            if (done() || ((tokenType != types) && ...)) {
                return false;
            }
            advance();
            return true;
        }

        auto advance() -> void;

        [[nodiscard]] auto peek() const -> token::Token;

        auto markCurrentToken() -> void {
            m_markedTokens.push(current());
        }

        [[nodiscard]] auto unmarkAndReturnCoords()
            -> token::Token::Coords;

        [[nodiscard]] auto current() const -> token::Token;

        [[nodiscard]] auto done() const -> bool {
            return m_started && m_tokenIt == m_tokens.end();
        }

    private:
        static auto makeInvalidToken() -> token::Token {
            return {token::EToken::Invalid, "", {0, 0}};
        }

    private:
        token::TokenizedBuffer const m_tokens;
        token::TokenizedBuffer::const_iterator m_tokenIt;
        Stack<token::Token> m_markedTokens{};
        bool m_started{};
    };
}

#endif // TLC_PARSE_STREAM_HPP
