#ifndef  TLC_SYNTAX_VISITOR_HPP
#define  TLC_SYNTAX_VISITOR_HPP

#include "core/core.hpp"
#include "forward.hpp"

namespace tlc::syntax {
  /**
   * Derived classes' operator() overloads must not be marked as 'const'
   * @tparam TReturn
   */
  template<typename TReturn = void>
  class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
  public:
    SyntaxTreeVisitor() = default;

  protected:
    using Visitor<Node, TReturn>::operator();

    template<typename S, typename N>
      requires(std::convertible_to<N, Node> && std::is_invocable_v<S, Node>)
    auto visitChildren(this S &&self, N const &node) -> auto {
      if constexpr (std::is_void_v<TReturn>) {
        rng::for_each(node.children(), [&self](auto const &child) {
          std::visit(self, child);
        });
      } else {
        return node.children() | rv::transform([&self](auto const &child) {
          return std::visit(self, child);
        }) | rng::to<Vec<TReturn> >();
      }
    }
  };
}

#endif // TLC_SYNTAX_VISITOR_HPP
