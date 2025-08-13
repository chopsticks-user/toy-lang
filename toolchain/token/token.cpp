export module token;

import lexeme;
import core;

namespace tlc::token {
    export class Token final {
    public:
        constexpr Token(lexeme::Lexeme type, StrV const str,
                        Location const location)
            : m_lexeme{std::move(type)}, m_str{str}, m_location{location} {}

        template <typename S>
        [[nodiscard]] auto lexeme(this S&& self) noexcept -> auto&& {
            return std::forward<S>(self).m_lexeme;
        }

        template <typename S>
        [[nodiscard]] auto str(this S&& self) noexcept -> auto&& {
            return std::forward<S>(self).m_str;
        }

        template <typename S>
        [[nodiscard]] auto location(this S&& self) noexcept -> auto&& {
            return std::forward<S>(self).m_location;
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

    export using TokenizedBuffer = Vec<Token>;

    // todo:
    // class TokenizedBuffer {
    // public:
    // private:
    // };
}
