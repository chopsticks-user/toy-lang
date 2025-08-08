#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::lexString() -> void {
        m_currentLexeme = lexeme::stringFragment;

        b8 escaped = false;
        b8 lastTokenIsPlaceholder = false;
        const auto toFragmentState = [&] {
            markTokenLocation();
            m_currentStr = "";
            m_currentLexeme = lexeme::stringFragment;
            lastTokenIsPlaceholder = false;
        };

        while (!m_stream.match(isStringTerminator) && m_stream.peek() != '\n') {
            m_stream.advance();
            if (lastTokenIsPlaceholder) {
                toFragmentState();
            }
            if (auto const c = m_stream.current(); escaped) {
                switch (c) {
                case 'n': {
                    appendStr('\n');
                    break;
                }
                case 't': {
                    appendStr('\t');
                    break;
                }
                case 'r': {
                    appendStr('\r');
                    break;
                }
                case 'b': {
                    appendStr('\b');
                    break;
                }
                case 'f': {
                    appendStr('\f');
                    break;
                }
                case 'a': {
                    appendStr('\a');
                    break;
                }
                case '\\': {
                    appendStr('\\');
                    break;
                }
                case '\'': {
                    appendStr('\'');
                    break;
                }
                case '"': {
                    appendStr('"');
                    break;
                }
                case '{': {
                    appendStr('{');
                    break;
                }
                case '}': {
                    appendStr('}');
                    break;
                }
                default: {
                    appendStr('\\');
                    appendStr(c);
                    break;
                }
                }
                escaped = false;
            }
            else if (c == '\\') {
                escaped = true;
            }
            else if (c == '{') {
                // consume string until the correct enclosing character '}' is found.
                // the consumed string will be "re-lexed" from a parser
                appendToken();
                markTokenLocation();
                m_currentStr = "";
                m_currentLexeme = lexeme::stringPlaceholder;

                szt left = 1, right = 0;
                c8 last = m_stream.current();
                while (left != right) {
                    m_stream.advance();

                    auto const cc = m_stream.current();
                    if (cc == '{' && last != '\\') {
                        ++left;
                    }
                    else if (cc == '}' && last != '\\') {
                        ++right;
                    }
                    else {
                        appendStr();
                    }
                    last = cc;
                }

                if (left != right) {
                    // todo: error
                }
                appendToken();
                lastTokenIsPlaceholder = true;
            }
            else {
                appendStr();
            }
        }

        if (lastTokenIsPlaceholder) {
            toFragmentState();
        }

        if (!isStringTerminator(m_stream.current())) {
            // todo: error
        }

        appendToken();
    }
}
