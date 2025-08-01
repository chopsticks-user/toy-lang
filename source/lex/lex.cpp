#include "lex.hpp"

#include "util.hpp"

namespace tlc::lex {
    auto Lex::operator()(fs::path const& filepath) -> token::TokenizedBuffer {
        return Lex{filepath}();
    }

    auto Lex::operator()(std::istringstream iss) -> token::TokenizedBuffer {
        return Lex{std::move(iss)}();
    }

    auto Lex::operator()() -> token::TokenizedBuffer {
        m_stream.consumeSpaces();
        while (!m_stream.done()) {
            m_currentStr = "";
            m_currentLexeme = lexeme::invalid;

            if (m_stream.match(isCommentOuter)) {
                markTokenCoords();
                appendStr();
                lexComment();
            }
            else if (m_stream.match(isDigit)) {
                markTokenCoords();
                appendStr();
                lexNumeric();
            }
            else if (m_stream.match(isLetter)) {
                markTokenCoords();
                appendStr();
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
