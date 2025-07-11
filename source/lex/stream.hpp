#ifndef TLC_LEX_STREAM_HPP
#define TLC_LEX_STREAM_HPP

#include "core/core.hpp"

namespace tlc::lex {
    class Stream {
        // todo: dynamic tabsize
        static constexpr szt tabSize = 4;

    public:
        Stream() = default;

        explicit Stream(fs::path const& filepath)
            : m_filereader(filepath) {
            // todo: eof
            m_currentChar = m_filereader.advance();
        }

        explicit Stream(std::istringstream iss)
            : m_filereader(std::move(iss)) {
            // todo: eof
            m_currentChar = m_filereader.advance();
        }

        auto match(std::same_as<c8> auto... expected) -> bool {
            if (m_filereader.eof() || ((m_filereader.peek() != expected) && ...)) {
                return false;
            }
            m_currentChar = m_filereader.advance();
            ++m_pos;
            return true;
        }

        auto match(bool (*cond)(c8)) -> bool {
            if (m_filereader.eof() || !cond(m_filereader.peek())) {
                return false;
            }
            m_currentChar = m_filereader.advance();
            ++m_pos;
            return true;
        }

        auto advance() -> void;

        auto consumeSpaces() -> void {}

        [[nodiscard]] auto done() const -> bool { return m_filereader.eof(); }

        [[nodiscard]] auto current() const -> c8 { return m_currentChar; }

    private:
        FileReader m_filereader;
        szt m_pos{}, m_line{}, m_column{};
        c8 m_currentChar{};
    };
}

#endif // TLC_LEX_STREAM_HPP
