#ifndef TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP
#define TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

namespace tlc::static_ {
    class TupleDiagnostic final : public syntax::SyntaxTreeVisitor<b8> {
    public:
        constexpr TupleDiagnostic() = default;
        using SyntaxTreeVisitor::operator();

        static constexpr auto operator()(syntax::Node const& node) -> void {
            node.visit(TupleDiagnostic{});
        }

    public:
        constexpr auto operator()(
            syntax::IsInternalNode auto const& node
        ) -> b8 {
            auto hasInnerTuples = rng::any_of(visitChildren(node),
                                              std::identity{});
            if constexpr (CanonicallyMatchesAnyOf<decltype(node),
                                                  syntax::expr::Tuple,
                                                  syntax::type::Tuple,
                                                  syntax::decl::Tuple>) {
                if (hasInnerTuples) {
                    // todo: collect error
                }
                return true;
            }
            return hasInnerTuples;
        }

        constexpr auto operator()(syntax::IsTerminalNode auto const&) -> b8 {
            return false;
        }
    };
}

#endif // TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP
