#ifndef TLC_STATIC_POST_PARSING_TUPLE_REDUCER_HPP
#define TLC_STATIC_POST_PARSING_TUPLE_REDUCER_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

namespace tlc::static_ {
    class SingletonTupleReducer final : public syntax::SyntaxTreeMutator {
    public:
        constexpr SingletonTupleReducer() = default;
        using SyntaxTreeMutator::operator();

        static constexpr auto operator()(syntax::Node& node) -> void {
            std::visit(SingletonTupleReducer{},
                       node, std::variant<Ref<syntax::Node>>{node});
        }

    public:
        constexpr auto operator()(syntax::expr::Tuple& ref,
                                  syntax::Node& node) -> void {
            visitChildren(ref);
            if (ref.size() == 1) {
                node = ref.firstChild();
            }
        }

        constexpr auto operator()(syntax::type::Tuple& ref,
                                  syntax::Node& node) -> void {
            visitChildren(ref);
            if (ref.size() == 1) {
                node = ref.firstChild();
            }
        }
    };
}

#endif // TLC_STATIC_POST_PARSING_TUPLE_REDUCER_HPP
