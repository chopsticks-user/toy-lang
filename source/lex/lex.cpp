#include "lex.hpp"

#include "util.hpp"

namespace tlc::lex {
    auto Lexer::operator()(fs::path const& filepath) -> token::TokenizedBuffer {
        return Lexer{filepath}();
    }

    auto Lexer::operator()(std::istringstream iss) -> token::TokenizedBuffer {
        return Lexer{std::move(iss)}();
    }

    auto Lexer::operator()() -> token::TokenizedBuffer {
        m_stream.consumeSpaces();
        while (!m_stream.done()) {
            m_currentLexeme = "";
            m_currentTokenType = token::EToken::Invalid;

            if (m_stream.match(isCommentOuter)) {
                markTokenCoords();
                appendLexeme();
                lexComment();
            }
            else if (m_stream.match(isDigit)) {
                markTokenCoords();
                appendLexeme();
                lexNumeric();
            }
            else if (m_stream.match(isLetter)) {
                markTokenCoords();
                appendLexeme();
                lexIdentifier();
            }
            else {
                m_stream.advance();
                markTokenCoords();
                lexSymbol();
            }

            m_stream.consumeSpaces();
        }

        return m_tokens;
    }
}
