#ifndef  TOYLANG_SYNTAX_BASE_HPP
#define  TOYLANG_SYNTAX_BASE_HPP

#include "Forward.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class Node {
  public:
    auto children() const noexcept -> const std::vector<VNode> &;

    auto childAt(sz index) const -> const VNode &;

  protected:
    explicit Node(std::vector<VNode> children) noexcept;

    auto childAt(sz index) -> VNode &;

  private:
    std::vector<VNode> m_children;
  };
}

#endif // TOYLANG_SYNTAX_BASE_HPP
