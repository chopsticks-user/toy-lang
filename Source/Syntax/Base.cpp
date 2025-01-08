#include "Base.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"
#include "Definitions.hpp"

namespace tl::syntax {
  ASTNodeBase::ASTNodeBase(std::vector<ASTNode> children) noexcept
    : m_children(std::move(children)) {
  }

  auto ASTNodeBase::children() const noexcept -> const std::vector<ASTNode> & {
    return m_children;
  }

  auto ASTNodeBase::childAt(sz index) const -> const ASTNode & {
    return m_children.at(index);
  }

  auto ASTNodeBase::firstChild() const -> CRef<ASTNode> {
    return m_children.front();
  }

  auto ASTNodeBase::lastChild() const -> CRef<ASTNode> {
    return m_children.back();
  }

  auto ASTNodeBase::childAt(sz index) -> ASTNode & {
    return m_children.at(index);
  }

  auto ASTNodeBase::firstChild() -> ASTNode & {
    return m_children.front();
  }

  auto ASTNodeBase::lastChild() -> ASTNode & {
    return m_children.back();
  }

  auto ASTNodeBase::nChildren() const noexcept -> sz {
    return m_children.size();
  }

  auto isEmptyAst(const ASTNode &node) -> bool {
    return std::holds_alternative<std::monostate>(node);
  }
}
