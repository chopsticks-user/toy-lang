#ifndef  TLC_SYNTAX_AST_VISITOR_HPP
#define  TLC_SYNTAX_AST_VISITOR_HPP

#include "core/core.hpp"
#include "forward.hpp"

namespace tlc::syntax {
  template<typename TDerived, typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
    template<typename TParentNode>
    static constexpr bool CallableWithChildNode =
        std::convertible_to<TParentNode, Node> &&
        requires
        {
          [] {
            static_assert(
              std::is_invocable_v<TDerived, Node>,
              "Requires adding 'public: using SyntaxTreeVisitor::operator();'"
              " to the visitor class body"
            );
          }();
        };

  protected:
    using Visitor<Node, TReturn>::operator();

    template<typename T>
      requires (CallableWithChildNode<T> && !std::is_void_v<TReturn>)
    auto visitChildren(T const &node) -> Vec<TReturn> {
      Vec<TReturn> results;
      results.reserve(node.nChildren());
      for (const auto &child: node.children()) {
        results.push_back(std::visit(*static_cast<TDerived *>(this), child));
      }
      return results;
    }

    template<typename T>
      requires CallableWithChildNode<T> && std::is_void_v<TReturn>
    auto visitChildren(T const &node) -> void {
      for (const auto &child: node.children()) {
        std::visit(*static_cast<TDerived *>(this), child);
      }
    }
  };
}

#endif // TLC_SYNTAX_AST_VISITOR_HPP
