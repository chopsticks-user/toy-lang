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
    template <typename TReturn = void>
    class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
    public:
        SyntaxTreeVisitor() = default;

    protected:
        using Visitor<Node, TReturn>::operator();

        // todo: prefix, infix, suffix
        template <typename S, IsASTNode N>
        constexpr auto visitChildren(this S&& self, N const& node) -> auto {
            if constexpr (std::is_void_v<TReturn>) {
                rng::for_each(node.children(), [&self](Node const& child) {
                    child.visit(std::forward<S>(self));
                });
            }
            else {
                return node.children()
                    | rv::transform([&self](Node const& child) {
                        return child.visit(std::forward<S>(self));
                    })
                    | rng::to<Vec<TReturn>>();
            }
        }
    };
}

#endif // TLC_SYNTAX_VISITOR_HPP
