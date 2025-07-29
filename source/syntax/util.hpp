#ifndef  TLC_SYNTAX_UTIL_HPP
#define  TLC_SYNTAX_UTIL_HPP

#include "core/core.hpp"
#include "token/token.hpp"
#include "base.hpp"

namespace tlc::syntax {
    auto isEmptyNode(Node const& node) -> bool;

    template <typename T>
    concept IsStrictlyASTNode =
        std::derived_from<T, detail::NodeBase> &&
        !std::same_as<detail::NodeBase, T>;

    template <typename T>
    concept IsASTNode = IsStrictlyASTNode<T> || std::same_as<Node, T>;

    // template<typename T, typename... U>
    // concept MatchASTNode = IsStrictlyASTNode<T> &&
    //                        (IsStrictlyASTNode<U> && ...) &&
    //                        (std::same_as<T, U> || ...)
    //                        || std::same_as<Node, T>;

    template <IsASTNode TNode>
    auto astCast(Node const& node, Str const& filepath = "") -> TNode {
        if constexpr (std::same_as<TNode, Node>) {
            return node;
        }

        if (!std::holds_alternative<TNode>(node)) {
            throw filepath.empty()
                      ? InternalError(filepath, "invalid AST-node cast")
                      : InternalError("invalid AST-node cast");
        }
        return std::get<TNode>(node);
    }

    template <std::derived_from<detail::NodeBase>... TNode>
    auto matchAstType(Node const& node) -> bool {
        return (std::holds_alternative<TNode>(node) || ...);
    }

    using OpPrecedence = szt;

    enum class EOperator {
        Prefix, Postfix, Binary, Ternary
    };

    auto isPrefixOperator(token::EToken type) -> bool;
    auto isPostfixStart(token::EToken type) -> bool;
    auto isBinaryOperator(token::EToken type) -> bool;
    auto opPrecedence(token::EToken tokenType, EOperator opType) -> OpPrecedence;
    auto isLeftAssociative(token::EToken type) -> bool;
    auto isAssignmentOperator(token::EToken type) -> bool;

    // const HashSet<Str> assignmentOps = {
    //   "=", "+=", "-=", "*=", "/=", "%=", "**=",
    //   "&=", "|=", "^=", "<<=", ">>=",
    // };
    // const HashSet<Str> overloadableOps = {
    //   "+", "-", "*", "/",
    //   "==", "!=",
    // };
}

#endif // TLC_SYNTAX_UTIL_HPP
