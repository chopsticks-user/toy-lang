#include "Base.hpp"
#include "Expressions.hpp"

namespace tl::parser::syntax {
  Node::Node(std::vector<VNode> children) noexcept
    : m_children(std::move(children)) {
  }

  auto Node::children() const noexcept -> const std::vector<VNode> & {
    return m_children;
  }

  auto Node::childAt(sz index) const -> const VNode & {
    return m_children.at(index);
  }

  auto Node::childAt(sz index) -> VNode & {
    return m_children.at(index);
  }
}
