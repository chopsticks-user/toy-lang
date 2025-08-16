#ifndef TLC_SYNTAX_VISITOR_HPP
#define TLC_SYNTAX_VISITOR_HPP

#include "core/core.hpp"
#include "forward.hpp"
#include "util.hpp"

namespace tlc::syntax {
    template <std::default_initializable TReturn>
    class SyntaxTreeVisitor : public Visitor<Node, TReturn> {
    protected:
        constexpr SyntaxTreeVisitor() = default;

        constexpr auto operator()(
            this IsChildOf<SyntaxTreeVisitor> auto&&, IsNode auto const&
        ) -> TReturn {
            return {};
        }

        [[nodiscard]] constexpr auto visitChildren(
            this IsChildOf<SyntaxTreeVisitor> auto&& self,
            IsInternalNode auto const& node
        ) -> Vec<TReturn> {
            return node.children()
                | rv::transform([&](Node const& child) {
                    return std::visit(std::forward<decltype(self)>(self), child);
                })
                | rng::to<Vec<TReturn>>();
        }
    };

    class SyntaxTreeMutator : public Visitor<Node> {
    protected:
        constexpr SyntaxTreeMutator() = default;

        constexpr auto operator()(
            this IsChildOf<SyntaxTreeMutator> auto&& self,
            [[maybe_unused]] IsNode auto& ref, Node&
        ) -> void {
            if constexpr (IsChildOf<decltype(ref), detail::InternalNodeBase>) {
                std::forward<decltype(self)>(self).visitChildren(ref);
            }
        }

        constexpr auto visitChildren(
            this IsChildOf<SyntaxTreeMutator> auto&& self,
            IsInternalNode auto& node
        ) -> void {
            rng::for_each(
                node.children(), [&](Node& child) {
                    std::visit(std::forward<decltype(self)>(self),
                               child, std::variant<Ref<Node>>{child});
                }
            );
        }
    };
}

#endif // TLC_SYNTAX_VISITOR_HPP
