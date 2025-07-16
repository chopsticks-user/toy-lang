#ifndef TLC_LEX_HPP
#define TLC_LEX_HPP

#include "core/core.hpp"
#include "token/token.hpp"

#include "stream.hpp"

namespace tlc::lex {
    class Lexer {
    public:
        // todo: reset before using
        auto operator()(fs::path const& filepath) -> Vec<token::Token>;
        auto operator()(std::istringstream iss) -> Vec<token::Token>;

    private:
        auto lex() -> Vec<token::Token>;
        auto lexComment() -> void;
        auto lexIdentifier() -> void;
        auto lexFloatingPoint() -> void;
        auto lexNondecimalNumeric() -> void;
        auto lexNumeric() -> void;
        auto lexSymbol() -> void;

    private:
        auto reset() -> void {
            m_stream = {};
            m_currentTokenType = {};
            m_currentLexeme = {};
            m_tokens = {};
        }

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
                m_tokenLine, m_tokenColumn
            );
        }

    private:
        Stream m_stream{};
        token::EToken m_currentTokenType{};
        Str m_currentLexeme{};
        szt m_tokenLine{}, m_tokenColumn{};
        Vec<token::Token> m_tokens{};
    };
}

#endif // TLC_LEX_HPP
