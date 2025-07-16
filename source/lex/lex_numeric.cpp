#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lexer::lexFloatingPoint() -> void {
        if (m_stream.match(isStartOfDecimalPart)) {
            appendLexeme();
            m_currentTokenType = token::EToken::FloatLiteral;
            szt count = 0;
            while (m_stream.match(isDigit)) {
                appendLexeme();
                ++count;
            }
            if (count == 0) {
                // todo: throw
                m_currentTokenType = token::EToken::Invalid;
            }
        }
    }

    auto Lexer::lexNondecimalNumeric() -> void {
        szt count = 0;
        if (m_stream.match('x')) {
            appendLexeme();
            m_currentTokenType = token::EToken::Integer16Literal;

            while (m_stream.match(isHexadecimalDigit)) {
                appendLexeme();
                ++count;
            }
        }
        else if (m_stream.match('b')) {
            appendLexeme();
            m_currentTokenType = token::EToken::Integer2Literal;

            while (m_stream.match(isBinaryDigit)) {
                appendLexeme();
                ++count;
            }
        }
        else {
            m_currentTokenType = token::EToken::Integer8Literal;

            while (m_stream.match(isOctalDigit)) {
                appendLexeme();
                ++count;
            }
            lexFloatingPoint();
        }

        if (count == 0) {
            // todo: throw
            m_currentTokenType = token::EToken::Invalid;
        }

        appendToken();
    }

    auto Lexer::lexNumeric() -> void {
        m_currentTokenType = token::EToken::Integer10Literal;
        if (m_stream.current() == '0') {
            return lexNondecimalNumeric();
        }

        while (m_stream.match(isDigit)) {
            appendLexeme();
        }
        lexFloatingPoint();

        appendToken();
    }
}
