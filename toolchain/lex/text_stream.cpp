export module lex:text_stream;

import core;

namespace tlc::lex {
    export class TextStream {
        // todo: dynamic tabsize
        static constexpr szt tabSize = 4;

    public:
        TextStream() = default;

        explicit TextStream(fs::path const& filepath)
            : m_filereader(filepath) {}

        explicit TextStream(std::istringstream iss)
            : m_filereader(std::move(iss)) {}

        [[nodiscard]] auto line() const -> szt { return m_line; }
        [[nodiscard]] auto column() const -> szt { return m_column; }

        auto match(std::same_as<char> auto... expected) -> bool {
            if (done() || ((m_filereader.peek() != expected) && ...)) {
                return false;
            }
            advance();
            return true;
        }

        auto match(bool (*cond)(char)) -> bool {
            if (done() || !cond(m_filereader.peek())) {
                return false;
            }
            advance();
            return true;
        }

        auto advance() -> void {
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

        [[nodiscard]] auto peek() const -> char {
            return m_filereader.peek();
        }

        auto consumeSpaces() -> void {
            while (match(' ', '\t', '\r', '\n')) {}
        }

        [[nodiscard]] auto done() const -> bool {
            return m_filereader.peek() == EOF;
        }

        [[nodiscard]] auto current() const -> char {
            return m_currentChar;
        }

    private:
        FileReader m_filereader;
        bool m_started{};
        szt m_pos{}, m_line{}, m_column{};
        char m_currentChar{};
    };
}
