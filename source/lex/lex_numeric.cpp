#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lexer::lexNondecimalNumeric() -> void {
        szt count = 0;
        if (m_stream.match('x')) {
            while (m_stream.match(isHexadecimalDigit)) {
                appendLexeme();
                ++count;
            }
        }
        else if (m_stream.match('b')) {
            while (m_stream.match(isBinaryDigit)) {
                appendLexeme();
                ++count;
            }
        }
        else {
            while (m_stream.match(isOctalDigit)) {
                appendLexeme();
                ++count;
            }
        }

        if (count == 0) {
            // todo: throw
            m_currentTokenType = token::EToken::Invalid;
        }

        appendToken();
    }

    auto Lexer::lexNumeric() -> void {
        m_currentTokenType = token::EToken::IntegerLiteral;
        if (m_stream.current() == '0') {
            return lexNondecimalNumeric();
        }

        while (m_stream.match(isDigit)) {
            appendLexeme();
        }
        if (m_stream.match(isStartOfDecimalPart)) {
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

        appendToken();
    }
}
