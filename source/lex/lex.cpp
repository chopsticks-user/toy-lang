#include "lex.hpp"

#include "util.hpp"

namespace tlc::lex {
    auto Lexer::operator()(fs::path const& filepath) -> Vec<token::Token> {
        m_stream = Stream{filepath};
        return lex();
    }

    auto Lexer::operator()(std::istringstream iss) -> Vec<token::Token> {
        m_stream = Stream{std::move(iss)};
        return lex();
    }

    auto Lexer::lex() -> Vec<token::Token> {
        while (!m_stream.done()) {
            m_stream.consumeSpaces();
            m_currentLexeme = "";

            if (m_stream.match(isCommentOuter)) {
                appendLexeme();
                lexComment();
                continue;
            }
            if (m_stream.match(isStringOuter)) {
                appendLexeme();
                lexString();
                continue;
            }
            if (m_stream.match(isDigit)) {
                appendLexeme();
                lexNumeric();
                continue;
            }
            if (m_stream.match(isLetter)) {
                appendLexeme();
                lexIdentifier();
                continue;
            }
            lexSymbol();
        }

        return m_tokens;
    }
}
