#include "stream.hpp"

namespace tlc::lex {
    auto Stream::advance() -> void {
        bool stop = false;
        do {
            if (auto const ch = m_filereader.advance();
                ch != m_filereader.eof()) {
                switch (ch) {
                case '\t': {
                    m_column += tabSize;
                    break;
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
                    stop = true;
                }
                }
                ++m_pos;
                m_currentChar = ch;
            }
        }
        while (!stop);
    }
}
