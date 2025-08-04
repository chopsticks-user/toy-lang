#ifndef TLC_TOKEN_IMPL_HPP
#define TLC_TOKEN_IMPL_HPP

#include "core/core.hpp"
#include "lexeme.hpp"

namespace tlc::token {
    class Token final {
    public:
        constexpr Token(lexeme::Lexeme type, StrV const str,
                        Location const location)
            : m_lexeme{std::move(type)}, m_str{str}, m_location{location} {}

        [[nodiscard]] auto lexeme() const noexcept -> lexeme::Lexeme {
            return m_lexeme;
        }

        [[nodiscard]] auto str() const noexcept -> Str {
            return m_str;
        }

        [[nodiscard]] auto location() const noexcept -> Location {
            return m_location;
        }

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_location.line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_location.column;
        }

    private:
        lexeme::Lexeme m_lexeme;
        Str m_str;
        Location m_location;
    };
}

#endif // TLC_TOKEN_IMPL_HPP
