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
        auto lexNondecimalNumeric() -> void;
        auto lexNumeric() -> void;
        auto lexSymbol() -> void;

        auto appendLexeme() -> void {
            m_currentLexeme += m_stream.current();
        }

        auto appendToken() -> void {
            m_tokens.emplace_back(
                m_currentTokenType, m_currentLexeme,
                m_stream.line(), m_stream.column()
            );
        }

    private:
        Stream m_stream{};
        token::EToken m_currentTokenType{};
        Str m_currentLexeme{};
        Vec<token::Token> m_tokens{};
    };
}

#endif // TLC_LEX_HPP
