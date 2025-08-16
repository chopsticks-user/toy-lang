#include "base.hpp"
#include "nodes.hpp"

namespace tlc::syntax::detail {
    InternalNodeBase::InternalNodeBase(
        Vec<Node> children, Location location
    ) : NodeBase{std::move(location)},
        m_children{std::move(children)} {}

    auto InternalNodeBase::children() const noexcept -> Span<Node const> {
        return m_children;
    }

    auto InternalNodeBase::children() noexcept -> Span<Node> {
        return m_children;
    }

    auto InternalNodeBase::childAt(const szt index) const -> const Node& {
        return m_children.at(index);
    }

    auto InternalNodeBase::firstChild() const -> Node const& {
        return m_children.front();
    }

    auto InternalNodeBase::lastChild() const -> Node const& {
        return m_children.back();
    }

    auto InternalNodeBase::childAt(szt const index) -> Node& {
        return m_children.at(index);
    }

    auto InternalNodeBase::firstChild() -> Node& {
        return m_children.front();
    }

    auto InternalNodeBase::lastChild() -> Node& {
        return m_children.back();
    }

    auto InternalNodeBase::nChildren() const noexcept -> szt {
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
            pathStr += "."s + Str(s);
        }

        return pathStr;
    }
}
