#ifndef TLC_SYNTAX_VISITOR_HPP
#define TLC_SYNTAX_VISITOR_HPP

#include "core/core.hpp"
#include "forward.hpp"
#include "util.hpp"

namespace tlc::syntax {
    /**
     * Derived classes' operator() overloads must not be marked as 'const'
     * @tparam TReturn
     */
    template <IsNonVoid TReturn>
    class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
    protected:
        constexpr SyntaxTreeVisitor() = default;
        using Visitor<Node, TReturn>::operator();

        template <IsChildOf<SyntaxTreeVisitor> S, IsNonTerminalASTNode N>
        constexpr auto visitChildren(this S&& self, N const& node)
            -> Vec<TReturn> {
            return node.children()
                | rv::transform([&self](Node const& child) {
                    return child.visit(std::forward<S>(self));
                })
                | rng::to<Vec<TReturn>>();
        }
    };

    class SyntaxTreeMutator : public Visitor<Node> {
    protected:
        constexpr SyntaxTreeMutator() = default;

        template <IsChildOf<SyntaxTreeMutator> S, IsNonTerminalASTNode N>
        constexpr auto visitChildren(this S&& self, N& node)
            -> void {
            rng::for_each(node.children(), [&self](Node& child) {
                std::visit(std::forward<S>(self),
                           child, std::variant<Ref<Node>>{child});
            });
        }

        template <IsChildOf<SyntaxTreeMutator> S>
        constexpr auto operator()(
            this S&& self, [[maybe_unused]] IsASTNode auto& ref, Node&
        ) -> void {
            if constexpr (IsChildOf<decltype(ref), detail::NodeBase>) {
                std::forward<S>(self).visitChildren(ref);
            }
        }
    };
}

#endif // TLC_SYNTAX_VISITOR_HPP
