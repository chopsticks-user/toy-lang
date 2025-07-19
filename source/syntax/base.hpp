#ifndef  TLC_SYNTAX_BASE_HPP
#define  TLC_SYNTAX_BASE_HPP

#include "core/core.hpp"
#include "forward.hpp"

namespace tlc::syntax::detail {
    class NodeBase {
    public:
        [[nodiscard]] auto children() const noexcept -> Span<Node const>;

        [[nodiscard]] auto children() noexcept -> Span<Node>;

        [[nodiscard]] auto childAt(szt index) const -> Node const&;

        [[nodiscard]] auto firstChild() const -> Node const&;

        [[nodiscard]] auto lastChild() const -> Node const&;

        [[nodiscard]] auto nChildren() const noexcept -> szt;

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_column;
        }

    protected:
        explicit NodeBase(Vec<Node> children, szt line = 0, szt column = 0) noexcept;

        auto childAt(szt index) -> Node&;

        auto firstChild() -> Node&;

        auto lastChild() -> Node&;

    private:
        Vec<Node> m_children;
        szt m_line{}, m_column{};
    };
}

#endif // TLC_SYNTAX_BASE_HPP
