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
        auto lexIdentifier() -> void;
        auto lexNumeric() -> void;
        auto lexSymbol() -> void;
        auto lexString() -> void;

        auto appendLexeme() -> void {
            m_currentLexeme += m_stream.current();
        }

    private:
        Stream m_stream{};
        Str m_currentLexeme{};
        Opt<token::Token> m_lastToken{};
    };
}

#endif // TLC_LEX_HPP
