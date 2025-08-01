#ifndef TLC_PARSE_STREAM_HPP
#define TLC_PARSE_STREAM_HPP

#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::parse {
    class TokenStream final {
    public:
        using MatchFn = bool (*)(lexeme::Lexeme const&);
        using TokenIt = token::TokenizedBuffer::const_iterator;

    public:
        explicit TokenStream(token::TokenizedBuffer tokens)
            : m_tokens{std::move(tokens)},
              m_tokenIt{m_tokens.begin()} {}

        auto match(std::same_as<lexeme::Lexeme> auto... types) -> bool {
            auto const tokenType = peek().lexeme();
            if (done() || ((tokenType != types) && ...)) {
                return false;
            }
            advance();
            return true;
        }

        auto match(MatchFn cond) -> bool;

        auto advance() -> void;

        [[nodiscard]] auto peek() const -> token::Token;

        auto markBacktrack() -> void {
            m_backtrack.push({m_tokenIt, m_started});
        }

        auto backtrack() -> void;

        [[nodiscard]] auto current() const -> token::Token;

        [[nodiscard]] auto done() const -> b8 {
            // todo:
            return m_started && m_tokenIt == m_tokens.end();
        }

    private:
        static auto makeInvalidToken() -> token::Token {
            return {lexeme::invalid, "", {0, 0}};
        }

    private:
        struct BacktrackStates {
            TokenIt tokenIt;
            b8 started;
        };

        token::TokenizedBuffer const m_tokens;
        TokenIt m_tokenIt;
        Stack<BacktrackStates> m_backtrack{};
        b8 m_started{};
    };
}

#endif // TLC_PARSE_STREAM_HPP
