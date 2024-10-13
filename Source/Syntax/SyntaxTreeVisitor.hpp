#ifndef  TOYLANG_SYNTAX_TREE_VISITOR_HPP
#define  TOYLANG_SYNTAX_TREE_VISITOR_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  template<typename TDerived, typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<
        TDerived, TReturn, VNode
      > {
  public:
    auto operator()(std::convertible_to<VNode> auto const &) -> TReturn {
      return {};
    }

  protected:
    using Super = SyntaxTreeVisitor;
  };
}

#endif // TOYLANG_SYNTAX_TREE_VISITOR_HPP
