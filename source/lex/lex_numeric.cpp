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
        }

        appendToken();
    }

    auto Lexer::lexNumeric() -> void {
        if (m_stream.current() == '0') {
            return lexNondecimalNumeric();
        }

        while (m_stream.match(isDigit)) {
            appendLexeme();
        }
        if (m_stream.match(isStartOfDecimalPart)) {
            szt count = 0;
            while (m_stream.match(isDigit)) {
                appendLexeme();
                ++count;
            }
            if (count == 0) {
                // todo: throw
            }
        }

        appendToken();
    }
}
