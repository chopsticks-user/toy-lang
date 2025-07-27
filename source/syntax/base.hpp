#ifndef  TLC_SYNTAX_BASE_HPP
#define  TLC_SYNTAX_BASE_HPP

#include "core/core.hpp"
#include "forward.hpp"
#include "token/token.hpp"

namespace tlc::syntax::detail {
    class NodeBase {
    protected:
        using Coords = token::Token::Coords;

    public:
        [[nodiscard]] auto children() const noexcept -> Span<Node const>;

        [[nodiscard]] auto children() noexcept -> Span<Node>;

        [[nodiscard]] auto childAt(szt index) const -> Node const&;

        [[nodiscard]] auto firstChild() const -> Node const&;

        [[nodiscard]] auto lastChild() const -> Node const&;

        [[nodiscard]] auto nChildren() const noexcept -> szt;

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_coords.first;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_coords.second;
        }

    protected:
        NodeBase(Vec<Node> children, token::Token::Coords coords) noexcept;

        auto childAt(szt index) -> Node&;

        auto firstChild() -> Node&;

        auto lastChild() -> Node&;

    private:
        Vec<Node> m_children;
        token::Token::Coords m_coords;
    };

    class IdentifierBase {
    public:
        explicit IdentifierBase(Vec<Str> path)
            : m_path{std::move(path)} {}

        [[nodiscard]] auto name() const noexcept -> Str {
            return m_path.empty() ? "" : m_path.back();
        }

        [[nodiscard]] auto path() const noexcept -> Str;

        [[nodiscard]] auto imported() const noexcept -> bool {
            return m_path.size() > 1;
        }

    protected:
        Vec<Str> m_path;
    };
}

#endif // TLC_SYNTAX_BASE_HPP
