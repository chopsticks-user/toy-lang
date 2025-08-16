#ifndef TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP
#define TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

namespace tlc::static_ {
    class TupleDiagnostic final : public syntax::SyntaxTreeVisitor<b8> {
    public:
        constexpr TupleDiagnostic() = default;
        using SyntaxTreeVisitor::operator();

        [[nodiscard]] static constexpr auto operator()(syntax::Node const& node)
            -> Vec<syntax::Node> {
            node.visit(TupleDiagnostic{});
            return {};
        }

    public:
        constexpr auto operator()(syntax::IsInternalNode auto const& node)
            -> b8 {
            auto hasInnerTuples = rng::any_of(visitChildren(node),
                                              std::identity{});
            if constexpr (CanonicallyMatchesAnyOf<
                decltype(node),
                syntax::expr::Tuple, syntax::type::Tuple, syntax::decl::Tuple
            >) {
                if (hasInnerTuples) {
                    // todo: collect error
                }
                return true;
            }
            return hasInnerTuples;
        }

        // private:
        //     Vec<Error<>>
    };
}

#endif // TLC_STATIC_POST_PARSING_TUPLE_DIAGNOSTIC_HPP
