#ifndef  TLC_SYNTAX_NODES_HPP
#define  TLC_SYNTAX_NODES_HPP

#include "core/core.hpp"
#include "base.hpp"
#include "token/token.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::NodeBase {
            Integer(i64 value, Coords coords);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::NodeBase {
            Float(f64 value, Coords coords);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct Boolean final : detail::NodeBase {
            Boolean(b8 value, Coords coords);

            [[nodiscard]] auto value() const noexcept -> b8 {
                return m_value;
            }

        private:
            b8 m_value;
        };

        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(Vec<Str> path, Coords coords);
        };

        struct Array final : detail::NodeBase {
            Array(Vec<Node> elements, Coords coords);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> elements, Coords coords);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct FnApp final : detail::NodeBase {
            FnApp(Node callee, Node args, Coords coords);

            [[nodiscard]] auto callee() const noexcept -> Node;

            [[nodiscard]] auto args() const noexcept -> Node;
        };

        struct Subscript final : detail::NodeBase {
            Subscript(Node collection, Node subscript, Coords coords);

            [[nodiscard]] auto collection() const noexcept -> Node;

            [[nodiscard]] auto subscript() const noexcept -> Node;
        };

        struct Access final : detail::NodeBase {
            Access(Node object, Str field, Coords coords);

            [[nodiscard]] auto object() const noexcept -> Node;

            [[nodiscard]] auto field() const noexcept -> Str;

        private:
            Str m_field;
        };

        struct Prefix final : detail::NodeBase {
            Prefix(Node operand, token::EToken op, Coords coords);

            [[nodiscard]] auto op() const noexcept -> token::EToken {
                return m_op;
            }

            [[nodiscard]] auto operand() const noexcept -> Node;

        private:
            token::EToken m_op;
        };

        struct Binary final : detail::NodeBase {
            Binary(Node lhs, Node rhs, token::EToken op, Coords coords);

            [[nodiscard]] auto op() const noexcept -> token::EToken {
                return m_op;
            }

            [[nodiscard]] auto left() const noexcept -> Node;

            [[nodiscard]] auto right() const noexcept -> Node;

        private:
            token::EToken m_op;
        };

        // struct String final : detail::NodeBase {
        //     explicit String(
        //         Str value,
        //         Vec<Node> placeholders
        //     );
        //
        //     [[nodiscard]] auto value() const noexcept -> StrV {
        //         return m_value;
        //     }
        //
        //     // auto placeholder(const sz index) const noexcept -> ASTNode const& {
        //     //   return childAt(index);
        //     // }
        //
        // private:
        //     Str m_value;
        // };

        // struct Ternary final : detail::NodeBase {
        //     Ternary(
        //         Node operand1,
        //         Node operand2,
        //         Node operand3,
        //         Str op1, Str op2
        //     );
        //
        //     [[nodiscard]] auto firstOp() const noexcept -> StrV {
        //         return m_op1;
        //     }
        //
        //     [[nodiscard]] auto secondOp() const noexcept -> StrV {
        //         return m_op2;
        //     }
        //
        // private:
        //     Str m_op1;
        //     Str m_op2;
        // };

        struct Record final : detail::NodeBase {
            Record(Node of, Vec<Pair<Str, Node>> entries, Coords coords);

            [[nodiscard]] auto size() const noexcept -> szt;

            [[nodiscard]] auto of() const noexcept -> Node;

            [[nodiscard]] auto key(szt const index) const noexcept -> Str {
                return m_keys.at(index);
            }

            [[nodiscard]] auto keys() const noexcept -> Span<const Str> {
                return m_keys;
            }

            [[nodiscard]] auto value(szt index) const noexcept -> Node const&;

        private:
            Vec<Str> m_keys;
        };
    }

    namespace type {
        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(Vec<Str> path, b8 fundamental, Coords coords);

            [[nodiscard]] auto fundamental() const noexcept -> bool {
                return m_fundamental;
            }

        private:
            b8 m_fundamental;
        };

        struct Array final : detail::NodeBase {
            Array(Node type, Vec<Node> sizes, Coords coords);

            [[nodiscard]] auto type() const noexcept -> Node const&;

            [[nodiscard]] auto size(szt dimIndex) const noexcept -> Node const&;

            [[nodiscard]] auto nDims() const noexcept -> szt;

            [[nodiscard]] auto fixed(szt dimIndex) const -> bool;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> types, Coords coords);

            [[nodiscard]] auto type(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };

        struct Function final : detail::NodeBase {
            Function(Node args, Node result, Coords coords);

            [[nodiscard]] auto args() const noexcept -> Node;

            [[nodiscard]] auto result() const noexcept -> Node;
        };

        /**
         * [[expr]]
         */
        struct Infer final : detail::NodeBase {
            Infer(Node expr, Coords coords);

            [[nodiscard]] auto expr() const noexcept -> Node;
        };

        /**
         * For both types and traits
         *
         * Int | Float | IsNumeric | ...
         */
        struct Sum final : detail::NodeBase {
            Sum(Vec<Node> types, Coords coords);

            [[nodiscard]] auto type(szt index) const -> Node;
        };

        /**
         * For traits only.
         *
         * IsNumeric & IsFundamental & ...
         */
        struct Product final : detail::NodeBase {
            Product(Vec<Node> types, Coords coords);

            [[nodiscard]] auto type(szt index) const -> Node;
        };
    }

    namespace decl {
        struct Identifier final : detail::NodeBase {
            Identifier(b8 constant, Node identifier, Node type, Coords coords);

            [[nodiscard]] auto constant() const noexcept -> b8 {
                return m_constant;
            }

            [[nodiscard]] auto identifier() const noexcept -> Node const&;

            [[nodiscard]] auto type() const noexcept -> Node const&;

        private:
            b8 m_constant;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> decls, Coords coords);

            [[nodiscard]] auto decl(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };
    }

    namespace stmt {
        struct Let final : detail::NodeBase {};

        struct Return final : detail::NodeBase {};

        /**
         * Syntax:
         *      \code
         *          "yield" expr? ";"
         *      \endcode
         *
         * Examples:
         *      \code
         *          export fn f: (n: Int) -> {
         *              let x = {
         *                  yield n * n;
         *              };
         *              ...
         *          }
         *      \endcode
         *
         * Notes:
         *      - Cannot be placed inside a 1st-level scope of a function.
         *      - Used to exit the current scope and indicate that the value evaluated
         *      by "expr" will be returned and effectively make the statement owning
         *      the exited scope an expression returning the value.
         *      - Unlike "return", "yield" cannot be a scope owner.
         *      - If "expr" is neither present nor evaluating to a non-void value,
         *      the type of the returned value is Void.
         *      - Like "return", "yield" is a pure statement.
         */
        struct Yield final : detail::NodeBase {
            Yield(Node expr, Coords coords);

            [[nodiscard]] auto expr() const noexcept -> Node const&;
        };

        /**
         * Syntax:
         *      \code
         *          "preface" ("{" stmt* "}" | expr ";")
         *      \endcode
         *
         * Examples:
         *
         * Notes:
         *      - Like its counter-part, "defer", "preface" is a pure statement
         *      and can be a scope owner.
         */
        struct Preface final : detail::NodeBase {};

        /**
         * Syntax:
         *      \code
         *          "defer" ("{" stmt* "}" | expr ";")
         *      \endcode
         *
         * Examples:
         *
         * Notes:
         *      - Like its counter-part, "preface", "defer" is a pure statement
         *      and can be a scope owner.
         */
        struct Defer final : detail::NodeBase {};

        struct Match final : detail::NodeBase {};

        struct Loop final : detail::NodeBase {};

        struct Cond final : detail::NodeBase {};

        struct Block final : detail::NodeBase {};

        struct Assign final : detail::NodeBase {};

        struct Expr final : detail::NodeBase {};
    }

    namespace global {}

    namespace def {}

    struct TranslationUnit final : detail::NodeBase {
        // explicit TranslationUnit(Vec<Node> definitions);
    };
}

#endif // TLC_SYNTAX_NODES_HPP
