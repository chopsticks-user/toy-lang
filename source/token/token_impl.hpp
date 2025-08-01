#ifndef TLC_TOKEN_IMPL_HPP
#define TLC_TOKEN_IMPL_HPP

#include "core/core.hpp"
#include "classify.hpp"

namespace tlc::token {
    class Token final {
    public:
        Token(lexeme::Lexeme type, StrV const str, Location coords)
            : m_lexeme{std::move(type)}, m_str{str}, m_coords{std::move(coords)} {}

        [[nodiscard]] auto lexeme() const noexcept -> lexeme::Lexeme {
            return m_lexeme;
        }

        [[nodiscard]] auto str() const noexcept -> Str {
            return m_str;
        }

        [[nodiscard]] auto coords() const noexcept -> Location {
            return m_coords;
        }

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_coords.line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_coords.column;
        }

    private:
        lexeme::Lexeme m_lexeme;
        Str m_str;
        Location m_coords;
    };
}

#endif // TLC_TOKEN_IMPL_HPP
