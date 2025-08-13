export module post_parsing:tuple_normalizer;

import syntax;

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
