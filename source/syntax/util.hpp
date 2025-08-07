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
                      ? InternalException(filepath, "invalid AST-node cast")
                      : InternalException("invalid AST-node cast");
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

#ifdef TLC_CONFIG_BUILD_TESTS
    extern const HashMap<lexeme::Lexeme, OpPrecedence> prefixOpPrecedenceTable;
    extern const HashMap<lexeme::Lexeme, OpPrecedence> binaryOpPrecedenceTable;
    extern const HashSet<lexeme::Lexeme> binaryTypeOpTable;
    extern const HashSet<lexeme::Lexeme> leftAssociativeOps;
    extern const HashSet<lexeme::Lexeme> assignmentOps;
#endif // TLC_CONFIG_BUILD_TESTS

    auto isPrefixOperator(lexeme::Lexeme const& lexeme) -> bool;
    auto isPostfixStart(lexeme::Lexeme const& lexeme) -> bool;
    auto isBinaryOperator(lexeme::Lexeme const& lexeme) -> bool;
    auto isBinaryTypeOperator(lexeme::Lexeme const& lexeme) -> bool;
    auto opPrecedence(lexeme::Lexeme const& lexeme, EOperator opType) -> OpPrecedence;
    auto isLeftAssociative(lexeme::Lexeme const& lexeme) -> bool;
    auto isAssignmentOperator(lexeme::Lexeme const& lexeme) -> bool;
}

#endif // TLC_SYNTAX_UTIL_HPP
