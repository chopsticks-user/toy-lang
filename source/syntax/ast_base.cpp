#include "ast_base.hpp"
#include "expr_nodes.hpp"
#include "stmt_nodes.hpp"
#include "definition_nodes.hpp"

namespace tlc::syntax {
  NodeBase::NodeBase(std::vector<Node> children) noexcept
    : m_children(std::move(children)) {
  }

  auto NodeBase::children() const noexcept -> Span<Node const> {
    return m_children;
  }

  auto NodeBase::view() const noexcept -> Span<Node const> {
    return m_children;
  }

  auto NodeBase::childAt(const sz index) const -> const Node & {
    return m_children.at(index);
  }

  auto NodeBase::firstChild() const -> Node const & {
    return m_children.front();
  }

  auto NodeBase::lastChild() const -> Node const & {
    return m_children.back();
  }

  auto NodeBase::childAt(sz index) -> Node & {
    return m_children.at(index);
  }

  auto NodeBase::firstChild() -> Node & {
    return m_children.front();
  }

  auto NodeBase::lastChild() -> Node & {
    return m_children.back();
  }

  auto NodeBase::nChildren() const noexcept -> sz {
    return m_children.size();
  }

  auto isEmptyAst(const Node &node) -> bool {
    return std::holds_alternative<std::monostate>(node);
  }
}
