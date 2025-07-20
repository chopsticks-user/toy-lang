#ifndef TLC_TOKEN_IMPL_HPP
#define TLC_TOKEN_IMPL_HPP

#include "core/core.hpp"
#include "classify.hpp"

namespace tlc::token {
    class Token final {
    public:
        using Coords = Pair<szt, szt>;

    public:
        Token(EToken const type, StrV const str, Coords coords)
            : m_type{type}, m_str{str}, m_coords{std::move(coords)} {}

        [[nodiscard]] auto type() const noexcept -> EToken {
            return m_type;
        }

        [[nodiscard]] auto str() const noexcept -> StrV {
            return m_str;
        }

        [[nodiscard]] auto coords() const noexcept -> Coords {
            return m_coords;
        }

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_coords.first;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_coords.second;
        }

    private:
        EToken m_type;
        Str m_str;
        Coords m_coords;
    };
}

#endif // TLC_TOKEN_IMPL_HPP
