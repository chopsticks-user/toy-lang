#ifndef  TLC_SYNTAX_UTIL_HPP
#define  TLC_SYNTAX_UTIL_HPP

#include "core/core.hpp"
#include "token/token.hpp"

#include "base.hpp"
#include "forward.hpp"
#include "nodes.hpp"

namespace tlc::syntax {
    namespace detail {
        template <typename T>
        concept IsCanonicalAndConvertibleToNode =
            IsCanonical<T> and std::convertible_to<T, Node>;

        template <typename T>
        concept IsInternalCandidate =
            IsCanonicallyChildOf<T, InternalNodeBase>;

        template <typename T>
        concept IsTerminalCandidate =
            IsCanonicallyChildOf<T, TerminalNodeBase> or
            MatchesAnyOf<T, std::monostate, RequiredButMissing>;
    }

    template <typename T>
    concept IsNode =
        detail::IsCanonicalAndConvertibleToNode<T> and
        detail::IsInternalCandidate<T> != detail::IsTerminalCandidate<T>;

    template <typename T>
    concept IsInternalNode =
        IsNode<T> and detail::IsInternalCandidate<T>;

    template <typename T>
    concept IsTerminalNode =
        IsNode<T> and detail::IsTerminalCandidate<T>;

    constexpr auto empty(Node const& node) -> bool {
        return std::holds_alternative<std::monostate>(node);
    }

    constexpr auto missing(Node const& node) -> bool {
        return std::holds_alternative<RequiredButMissing>(node);
    }

    template <IsNode TNode>
    constexpr auto cast(Node const& node, StrV const filepath = "")
        -> TNode {
        if constexpr (std::same_as<TNode, Node>) {
            return node;
        }

        if (!std::holds_alternative<TNode>(node)) {
            exitOnInternalError(
                std::format("invalid AST-node cast where filepath = {}",
                            filepath)
            );
        }
        return std::get<TNode>(node);
    }

    template <IsNode... TNode>
    constexpr auto match(Node const& node) -> bool {
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

    class NodeWrapper {
    public:
        constexpr NodeWrapper() = default;

        explicit constexpr NodeWrapper(Node node) noexcept
            : m_node{std::move(node)} {}

        explicit constexpr operator bool() const noexcept {
            return empty();
        }

        constexpr auto empty() const noexcept -> bool {
            return std::holds_alternative<std::monostate>(m_node);
        }

        constexpr auto missing() const noexcept -> bool {
            return std::holds_alternative<RequiredButMissing>(m_node);
        }

    private:
        Node m_node;
    };
}

#endif // TLC_SYNTAX_UTIL_HPP
