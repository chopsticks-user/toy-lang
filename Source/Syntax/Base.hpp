#ifndef  TOYLANG_SYNTAX_BASE_HPP
#define  TOYLANG_SYNTAX_BASE_HPP

#include "Forward.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class ASTNodeBase {
  public:
    auto children() const noexcept -> const std::vector<ASTNode> &;

    auto childAt(sz index) const -> const ASTNode &;

    auto nChildren() const noexcept -> sz;

  protected:
    explicit ASTNodeBase(std::vector<ASTNode> children) noexcept;

    auto childAt(sz index) -> ASTNode &;

    auto firstChild() -> ASTNode &;

    auto lastChild() -> ASTNode &;

  private:
    std::vector<ASTNode> m_children;
  };

  auto isEmpty(const ASTNode &node) -> bool;
}

#endif // TOYLANG_SYNTAX_BASE_HPP
