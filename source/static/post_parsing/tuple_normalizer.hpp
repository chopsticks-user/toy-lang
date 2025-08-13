#ifndef TLC_STATIC_POST_PARSING_TUPLE_NORMALIZER_HPP
#define TLC_STATIC_POST_PARSING_TUPLE_NORMALIZER_HPP

#include "core/core.hpp"
#include "syntax/syntax.cpp"

namespace tlc::static_ {
    class TupleNormalizer final :
        public syntax::SyntaxTreeVisitor<Opt<syntax::Node>> {
    public:
        using SyntaxTreeVisitor::operator();

        static auto operator()(syntax::Node& node) -> void {
            std::visit(TupleNormalizer{}, node);
        }

    public:
        constexpr auto operator()(syntax::expr::Tuple& node)
            -> Opt<syntax::Node> {
            if (m_hasOuterTupleExpr) {
                // todo: error
            }
            if (node.size() == 1) {}
            return {};
        }

    private:
        b8 m_hasOuterTupleExpr = false;
    };
}

#endif // TLC_STATIC_POST_PARSING_TUPLE_NORMALIZER_HPP
