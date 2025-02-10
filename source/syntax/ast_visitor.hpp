#ifndef  TLC_SYNTAX_AST_VISITOR_HPP
#define  TLC_SYNTAX_AST_VISITOR_HPP

#include "core/core.hpp"
#include "forward.hpp"

namespace tlc::syntax {
  template<typename TParentNode, typename TDerived>
  concept AstVisitorCallableWithChildNode =
      std::convertible_to<TParentNode, Node> &&
      std::is_invocable_v<TDerived, Node>;

  template<typename TParentNode, typename TDerived, typename TReturn>
  concept AstVisitorVoidOverload =
      AstVisitorCallableWithChildNode<TParentNode, TDerived> &&
      std::is_void_v<TReturn>;

  template<typename TParentNode, typename TDerived, typename TReturn>
  concept AstVisitorNonVoidOverload =
      AstVisitorCallableWithChildNode<TParentNode, TDerived> &&
      !std::is_void_v<TReturn>;

  template<typename TDerived, typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
    template<typename TParentNode>
    static constexpr bool CallableWithChildNode =
        std::convertible_to<TParentNode, Node> &&
        std::is_invocable_v<TDerived, Node>;

  protected:
    using Visitor<Node, TReturn>::operator();

    template<AstVisitorNonVoidOverload<TDerived, TReturn> T>
    auto visitChildren(T const &node) -> Vec<TReturn> {
      return node.children() |
             rv::transform([this](auto const &child) {
               return std::visit(*static_cast<TDerived *>(this), child);
             }) |
             rng::to<Vec<TReturn> >();
    }

    template<AstVisitorVoidOverload<TDerived, TReturn> T>
    auto visitChildren(T const &node) -> void {
      node.children() | rng::for_each([this](auto const &child) {
        std::visit(*static_cast<TDerived *>(this), child);
      });
    }
  };
}

#endif // TLC_SYNTAX_AST_VISITOR_HPP
