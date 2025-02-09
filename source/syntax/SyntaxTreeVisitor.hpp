#ifndef  TOYLANG_SYNTAX_TREE_VISITOR_HPP
#define  TOYLANG_SYNTAX_TREE_VISITOR_HPP

#include "core/core.hpp"

namespace tl::syntax {
  template<typename TDerived, typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<ASTNode, TReturn> {
    static constexpr bool ActivateVoidOverload = std::is_void_v<TReturn>;
    static constexpr bool ActivateNonVoidOverload = !std::is_void_v<TReturn>;

    template<typename TParentNode>
    static constexpr bool IsCallableWithASTChildNode =
        std::convertible_to<TParentNode, ASTNode> &&
        requires
        {
          [] {
            static_assert(
              std::is_invocable_v<TDerived, ASTNode>,
              "Requires adding 'public: using SyntaxTreeVisitor::operator();' to the visitor class body"
            );
          }();
        };

  protected:
    using Visitor<ASTNode, TReturn>::operator();

    template<typename T>
      requires IsCallableWithASTChildNode<T> && ActivateNonVoidOverload
    auto visitChildren(T const &node) -> std::vector<TReturn> {
      std::vector<TReturn> results;
      results.reserve(node.nChildren());
      for (const auto &child: node.children()) {
        results.push_back(std::visit(*static_cast<TDerived *>(this), child));
      }
      return results;
    }

    template<typename T>
      requires IsCallableWithASTChildNode<T> && ActivateVoidOverload
    auto visitChildren(T const &node) -> void {
      for (const auto &child: node.children()) {
        std::visit(*static_cast<TDerived *>(this), child);
      }
    }
  };
}

#endif // TOYLANG_SYNTAX_TREE_VISITOR_HPP
