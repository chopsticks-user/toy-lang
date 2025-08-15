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
        SyntaxTreeVisitor() = default;
        using Visitor<Node, TReturn>::operator();

        // todo: prefix, infix, suffix
        template <typename S, IsASTNode N>
        constexpr auto visitChildren(this S&& self, N const& node) -> auto {
            return node.children()
                | rv::transform([&self](Node const& child) {
                    return child.visit(std::forward<S>(self));
                })
                | rng::to<Vec<TReturn>>();
        }
    };

    class SyntaxTreeMutator : public Visitor<Node> {
    protected:
        SyntaxTreeMutator() = default;
        using Visitor::operator();

        template <typename S, IsASTNode N>
        constexpr auto visitChildren(this S&& self, N& node) -> void {
            rng::for_each(node.children(), [&self](Node& child) {
                child.visit(std::forward<S>(self), std::variant<Node>{});
            });
        }
    };
}

#endif // TLC_SYNTAX_VISITOR_HPP
