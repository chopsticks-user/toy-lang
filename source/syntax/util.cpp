export module syntax:util;

import :nodes;
import lexeme;
#include "core/core.hpp"

namespace tlc::syntax {
    export {
        constexpr auto empty(Node const& node) -> bool {
            return std::holds_alternative<std::monostate>(node);
        }

        constexpr auto missing(Node const& node) -> bool {
            return std::holds_alternative<RequiredButMissing>(node);
        }

        template <typename T>
        concept IsStrictlyASTNode =
            std::derived_from<T, detail::NodeBase> &&
            !std::same_as<detail::NodeBase, T>;

        template <typename T>
        concept IsASTNode = IsStrictlyASTNode<T> || std::same_as<Node, T>;

        template <IsASTNode TNode>
        constexpr auto cast(Node const& node, StrV const filepath = "")
            -> TNode {
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
        constexpr auto match(Node const& node) -> bool {
            return (std::holds_alternative<TNode>(node) || ...);
        }
    }

    export {
        using OpPrecedence = szt;

        enum class EOperator {
            Prefix, Postfix, Binary, Ternary
        };
    }

    // todo: check C operator precedence
    const HashMap<lexeme::Lexeme, OpPrecedence>
    prefixOpPrecedenceTable = {
        {lexeme::exclaim, 40},
        {lexeme::tilde, 41},
        {lexeme::plus, 42},
        {lexeme::minus, 43},
        {lexeme::hash, 44},
        {lexeme::dot3, 45},
        {lexeme::ampersand, 46},
    };

    const HashMap<lexeme::Lexeme, OpPrecedence>
    binaryOpPrecedenceTable = {
        {lexeme::bar2, 10},
        {lexeme::ampersand2, 12},
        {lexeme::bar, 14},
        {lexeme::hat, 16},
        {lexeme::ampersand, 18},
        {lexeme::exclaimEqual, 20},
        {lexeme::equal2, 20},
        {lexeme::greaterEqual, 22},
        {lexeme::lessEqual, 22},
        {lexeme::greater, 22},
        {lexeme::less, 22},
        {lexeme::less2, 24},
        {lexeme::greater2, 24},
        {lexeme::dot2, 26},
        {lexeme::minus, 28},
        {lexeme::plus, 28},
        {lexeme::fwdSlash, 30},
        {lexeme::star, 30},
        {lexeme::star2, 32},
        {lexeme::barGreater, 34},
    };

    const HashSet<lexeme::Lexeme> binaryTypeOpTable = {
        lexeme::bar, lexeme::ampersand,
        lexeme::equal2, lexeme::exclaimEqual,
        lexeme::barGreater,
    };

    const HashSet<lexeme::Lexeme> leftAssociativeOps = {
        lexeme::bar2, lexeme::ampersand2, lexeme::bar, lexeme::hat,
        lexeme::ampersand, lexeme::exclaimEqual, lexeme::equal2,
        lexeme::greaterEqual, lexeme::lessEqual, lexeme::greater,
        lexeme::less, lexeme::less2, lexeme::greater2, lexeme::plus,
        lexeme::minus, lexeme::star, lexeme::fwdSlash, lexeme::star2,
        lexeme::barGreater, lexeme::dot2,
    };

    const HashSet<lexeme::Lexeme> assignmentOps = {
        lexeme::plusEqual, lexeme::minusEqual, lexeme::starEqual,
        lexeme::fwdSlashEqual, lexeme::percentEqual, lexeme::star2Equal,
        lexeme::ampersandEqual, lexeme::barEqual, lexeme::hatEqual,
        lexeme::less2Equal, lexeme::greater2Equal, lexeme::colonEqual,
    };

    const HashSet postfixStart = {
        lexeme::leftParen, lexeme::leftBracket,
    };

    export {
        constexpr auto isPrefixOperator(lexeme::Lexeme const& lexeme) -> bool {
            return prefixOpPrecedenceTable.contains(lexeme);
        }

        constexpr auto isPostfixStart(lexeme::Lexeme const& lexeme) -> bool {
            return postfixStart.contains(lexeme);
        }

        constexpr auto isBinaryOperator(lexeme::Lexeme const& lexeme) -> bool {
            return binaryOpPrecedenceTable.contains(lexeme);
        }

        constexpr auto isBinaryTypeOperator(lexeme::Lexeme const& lexeme) -> bool {
            return binaryTypeOpTable.contains(lexeme);
        }

        constexpr auto opPrecedence(
            lexeme::Lexeme const& lexeme, EOperator const opType
        ) -> OpPrecedence {
            switch (opType) {
            case EOperator::Prefix: return prefixOpPrecedenceTable.at(lexeme);
            case EOperator::Binary: return binaryOpPrecedenceTable.at(lexeme);
            default: return 0;
            }
        }

        constexpr auto isLeftAssociative(lexeme::Lexeme const& lexeme) -> b8 {
            return leftAssociativeOps.contains(lexeme);
        }

        constexpr auto isAssignmentOperator(lexeme::Lexeme const& lexeme) -> b8 {
            return assignmentOps.contains(lexeme);
        }
    }

    // class NodeWrapper {
    // public:
    //     constexpr NodeWrapper() = default;
    //
    //     explicit constexpr NodeWrapper(Node node) noexcept
    //         : m_node{std::move(node)} {}
    //
    //     explicit constexpr operator bool() const noexcept {
    //         return empty();
    //     }
    //
    //     constexpr auto empty() const noexcept -> bool {
    //         return std::holds_alternative<std::monostate>(m_node);
    //     }
    //
    //     constexpr auto missing() const noexcept -> bool {
    //         return std::holds_alternative<RequiredButMissing>(m_node);
    //     }
    //
    // private:
    //     Node m_node;
    // };
}
