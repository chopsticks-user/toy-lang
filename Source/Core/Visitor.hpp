#ifndef  TOYLANG_CORE_VISITOR_HPP
#define  TOYLANG_CORE_VISITOR_HPP

#include "Types.hpp"

namespace tl {
  template<typename Derived, typename... TVisitable>
  class Visitor {
  public:
    using Visitable = std::variant<TVisitable...>;

  protected:
    Visitor() = default;

    void visitChildren(std::convertible_to<Visitable> auto const &node) {
      for (const auto &child: node.children()) {
        std::visit(static_cast<Derived &>(*this), child);
      }
    }
  };

  template<typename TVisitor>
  auto visit(std::convertible_to<typename TVisitor::Visitable> auto &&visitable) {
    return std::visit(TVisitor{}, std::forward<typename TVisitor::Visitable>(visitable));
  }
}

#endif // TOYLANG_CORE_VISITOR_HPP
