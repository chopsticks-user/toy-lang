#ifndef  TLC_SYNTAX_BASE_HPP
#define  TLC_SYNTAX_BASE_HPP

#include "core/core.hpp"
#include "forward.hpp"
#include "token/token.hpp"

namespace tlc::syntax::detail {
    class NodeBase {
    public:
        [[nodiscard]] auto children() const noexcept -> Span<Node const>;

        [[nodiscard]] auto children() noexcept -> Span<Node>;

        [[nodiscard]] auto childAt(szt index) const -> Node const&;

        [[nodiscard]] auto childAt(szt index) -> Node&;

        [[nodiscard]] auto firstChild() const -> Node const&;

        [[nodiscard]] auto firstChild() -> Node&;

        [[nodiscard]] auto lastChild() const -> Node const&;

        [[nodiscard]] auto lastChild() -> Node&;

        [[nodiscard]] auto nChildren() const noexcept -> szt;

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_location.line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_location.column;
        }

    protected:
        NodeBase(Vec<Node> children, Location coords) noexcept;

    private:
        Vec<Node> m_children;
        Location m_location;
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

        [[nodiscard]] auto anonymous() const noexcept -> bool {
            return m_path.size() > 1;
        }

    protected:
        Vec<Str> m_path;
    };
}

#endif // TLC_SYNTAX_BASE_HPP
