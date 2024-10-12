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
      for (const auto &ptr: node.children()) {
        std::visit(static_cast<Derived &>(*this), *ptr);
      }
    }
  };

  // template<typename T>
  // concept IsVisitor = std::same_as<T, PrettyPrinter> && std::constructible_from<T>;

  template<typename TVisitor>
  auto visit(typename TVisitor::Visitable &&visitable) {
    return std::visit(TVisitor{}, std::forward<TVisitor::Visitable>(visitable));
  }
}

#endif // TOYLANG_CORE_VISITOR_HPP
