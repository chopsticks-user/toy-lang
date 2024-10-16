#ifndef  TOYLANG_CORE_VISITOR_HPP
#define  TOYLANG_CORE_VISITOR_HPP

#include "Types.hpp"

namespace tl {
  template<typename Derived, typename TReturn, typename TVisitable>
  class Visitor {
  public:
    using Visitable = TVisitable;

    auto operator()(std::convertible_to<Visitable> auto const &node) -> TReturn {
      return {};
    }

  protected:
    using Super = Visitor;

    Visitor() = default;

    auto visitChildren(std::convertible_to<Visitable> auto const &node) -> std::vector<TReturn> {
      std::vector<TReturn> results;
      results.reserve(node.nChildren());
      for (const auto &child: node.children()) {
        results.push_back(std::visit(*static_cast<Derived *>(this), child));
      }
      return results;
    }
  };

  template<typename TVisitor>
  auto visit(std::convertible_to<typename TVisitor::Visitable> auto &&visitable) {
    return std::visit(TVisitor{}, std::forward<typename TVisitor::Visitable>(visitable));
  }
}

#endif // TOYLANG_CORE_VISITOR_HPP
