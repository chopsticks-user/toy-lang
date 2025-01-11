#ifndef  TOYLANG_SYNTAX_BASE_HPP
#define  TOYLANG_SYNTAX_BASE_HPP

#include "Forward.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class ASTNodeBase {
  public:
    auto children() const noexcept -> CRef<Vec<ASTNode> >;

    auto view() const noexcept -> Vec<ASTNode>;

    auto childAt(sz index) const -> CRef<ASTNode>;

    auto firstChild() const -> CRef<ASTNode>;

    auto lastChild() const -> CRef<ASTNode>;

    auto nChildren() const noexcept -> sz;

  protected:
    explicit ASTNodeBase(std::vector<ASTNode> children) noexcept;

    auto childAt(sz index) -> ASTNode &;

    auto firstChild() -> ASTNode &;

    auto lastChild() -> ASTNode &;

  private:
    std::vector<ASTNode> m_children;
  };

  auto isEmptyAst(CRef<ASTNode> node) -> bool;

  template<std::derived_from<ASTNodeBase> TNode>
  auto astCast(CRef<ASTNode> node, CRef<String> filepath = "") -> TNode {
    if (!std::holds_alternative<TNode>(node)) {
      throw filepath.empty()
              ? InternalException(filepath, "invalid AST-node cast")
              : InternalException("invalid AST-node cast");
    }
    return std::get<TNode>(node);
  }

  template<std::derived_from<ASTNodeBase>... TNode>
  auto matchAstType(CRef<ASTNode> node) -> bool {
    return !((std::holds_alternative<TNode>(node)) || ...);
  }
}

#endif // TOYLANG_SYNTAX_BASE_HPP
