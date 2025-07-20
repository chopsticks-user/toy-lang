#ifndef TLC_LEX_HPP
#define TLC_LEX_HPP

#include "core/core.hpp"
#include "token/token.hpp"

#include "stream.hpp"

namespace tlc::lex {
    class Lexer final {
    public:
        static auto operator()(fs::path const& filepath) -> token::TokenizedBuffer;
        static auto operator()(std::istringstream iss) -> token::TokenizedBuffer;

        explicit Lexer(fs::path const& filepath)
            : m_stream{filepath} {}

        explicit Lexer(std::istringstream iss)
            : m_stream{std::move(iss)} {}

        auto operator()() -> token::TokenizedBuffer;

    private:
        auto lexComment() -> void;
        auto lexIdentifier() -> void;
        auto lexFloatingPoint() -> void;
        auto lexNondecimalNumeric() -> void;
        auto lexNumeric() -> void;
        auto lexSymbol() -> void;

    private:
        auto classifyIdentifier(StrV lexeme) -> void;

        auto markTokenCoords() -> void {
            m_tokenLine = m_stream.line();
            m_tokenColumn = m_stream.column();
        }

        auto appendLexeme() -> void {
            m_currentLexeme += m_stream.current();
        }

        auto appendToken() -> void {
            if (m_currentTokenType == token::EToken::Invalid) {
                // todo: throw
                return;
            }

            m_tokens.emplace_back(
                m_currentTokenType, m_currentLexeme,
                token::Token::Coords{m_tokenLine, m_tokenColumn}
            );
        }

    private:
        Stream m_stream;
        token::EToken m_currentTokenType{};
        Str m_currentLexeme{};
        szt m_tokenLine{}, m_tokenColumn{};
        token::TokenizedBuffer m_tokens{};
    };
}

#endif // TLC_LEX_HPP
