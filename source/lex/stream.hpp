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
            : m_filereader(filepath) {}

        explicit Stream(std::istringstream iss)
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

        auto advance() -> void;

        [[nodiscard]] auto peek() const -> char {
            return m_filereader.peek();
        }

        auto consumeSpaces() -> void;

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

#endif // TLC_LEX_STREAM_HPP
