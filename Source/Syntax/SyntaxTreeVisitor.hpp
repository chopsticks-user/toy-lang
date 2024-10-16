#ifndef  TOYLANG_SYNTAX_TREE_VISITOR_HPP
#define  TOYLANG_SYNTAX_TREE_VISITOR_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  template<typename TDerived, typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<
        TDerived, TReturn, VNode
      > {
  protected:
    using Super = SyntaxTreeVisitor;
  };
}

#endif // TOYLANG_SYNTAX_TREE_VISITOR_HPP
