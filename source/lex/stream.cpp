#include "stream.hpp"

namespace tlc::lex {
    auto Stream::advance() -> void {
        if (done()) {
            return;
        }

        // todo: comments
        if (m_started) {
            switch (m_currentChar) {
            case '\t': {
                m_column += tabSize;
                break;
            }
            case '\r': {
                m_filereader.skipLine();
                [[fallthrough]];
            }
            case '\n': {
                m_column = 0;
                ++m_line;
                break;
            }
            case ' ': {
                ++m_column;
                break;
            }
            default: {
                ++m_column;
            }
            }
        }

        if (auto const c = m_filereader.advance();
            c != m_filereader.eof()) {
            m_currentChar = c;
            if (!m_started) {
                m_started = true;
                return;
            }
            ++m_pos;
        }
    }

    auto Stream::consumeSpaces() -> void {
        while (match(' ', '\t', '\r', '\n')) {}
    }
}
