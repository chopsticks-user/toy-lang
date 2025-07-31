#include "base.hpp"
#include "nodes.hpp"

namespace tlc::syntax::detail {
    NodeBase::NodeBase(Vec<Node> children, Location coords) noexcept
        : m_children(std::move(children)), m_coords(std::move(coords)) {}

    auto NodeBase::children() const noexcept -> Span<Node const> {
        return m_children;
    }

    auto NodeBase::children() noexcept -> Span<Node> {
        return m_children;
    }

    auto NodeBase::childAt(const szt index) const -> const Node& {
        return m_children.at(index);
    }

    auto NodeBase::firstChild() const -> Node const& {
        return m_children.front();
    }

    auto NodeBase::lastChild() const -> Node const& {
        return m_children.back();
    }

    auto NodeBase::childAt(szt const index) -> Node& {
        return m_children.at(index);
    }

    auto NodeBase::firstChild() -> Node& {
        return m_children.front();
    }

    auto NodeBase::lastChild() -> Node& {
        return m_children.back();
    }

    auto NodeBase::nChildren() const noexcept -> szt {
        return m_children.size();
    }

    auto IdentifierBase::path() const noexcept -> Str {
        if (m_path.empty()) {
            return "";
        }

        Str pathStr = m_path.front();

        if (!imported()) {
            return pathStr;
        }

        for (StrV s : m_path | rv::drop(1)) {
            pathStr += "::"s + Str(s);
        }

        return pathStr;
    }
}
