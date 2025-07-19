#include "base.hpp"
#include "nodes.hpp"

namespace tlc::syntax::detail {
    NodeBase::NodeBase(Vec<Node> children, szt line, szt column) noexcept
        : m_children(std::move(children)) {}

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
}
