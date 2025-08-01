#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::lexFloatingPoint() -> void {
        if (m_stream.match(isStartOfDecimalPart)) {
            appendStr();
            m_currentLexeme = lexeme::floatLiteral;
            szt count = 0;
            while (m_stream.match(isDigit)) {
                appendStr();
                ++count;
            }
            if (count == 0) {
                // todo: throw
                m_currentLexeme = lexeme::invalid;
            }
        }
    }

    auto Lex::lexNondecimalNumeric() -> void {
        szt count = 0;
        if (m_stream.match('x')) {
            appendStr();
            m_currentLexeme = lexeme::integer16Literal;

            while (m_stream.match(isHexadecimalDigit)) {
                appendStr();
                ++count;
            }
        }
        else if (m_stream.match('b')) {
            appendStr();
            m_currentLexeme = lexeme::integer2Literal;

            while (m_stream.match(isBinaryDigit)) {
                appendStr();
                ++count;
            }
        }
        else {
            m_currentLexeme = lexeme::integer8Literal;

            while (m_stream.match(isOctalDigit)) {
                appendStr();
                ++count;
            }
            lexFloatingPoint();

            if (m_currentStr.length() > count + 1) {
                m_currentLexeme = lexeme::floatLiteral;
            }
        }

        if (count == 0 && m_currentLexeme != lexeme::floatLiteral) {
            // simply "0"
            m_currentLexeme = lexeme::integer10Literal;
        }

        appendToken();
    }

    auto Lex::lexNumeric() -> void {
        m_currentLexeme = lexeme::integer10Literal;
        if (m_stream.current() == '0') {
            return lexNondecimalNumeric();
        }

        while (m_stream.match(isDigit)) {
            appendStr();
        }
        lexFloatingPoint();

        appendToken();
    }
}
