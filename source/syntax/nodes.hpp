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
            Record(Node type, Vec<Pair<Str, Node>> entries, Coords coords);

            [[nodiscard]] auto size() const noexcept -> szt;

            [[nodiscard]] auto type() const noexcept -> Node;

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
            Identifier(b8 constant, Str name, Node type, Coords coords);

            [[nodiscard]] auto constant() const noexcept -> b8 {
                return m_constant;
            }

            [[nodiscard]] auto name() const noexcept -> Str const& {
                return m_name;
            }

            [[nodiscard]] auto type() const noexcept -> Node const&;

            [[nodiscard]] auto inferred() const noexcept -> b8;

        private:
            b8 m_constant;
            Str m_name;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> decls, Coords coords);

            [[nodiscard]] auto decl(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };
    }

    namespace stmt {
        struct Let final : detail::NodeBase {
            Let(Node decl, Node initializer, Coords coords);

            [[nodiscard]] auto decl() const noexcept -> Node const&;

            [[nodiscard]] auto initializer() const noexcept -> Node const&;

            [[nodiscard]] auto defaultInitialized() const -> bool;
        };

        struct Return final : detail::NodeBase {
            Return(Node expr, Coords coords);

            [[nodiscard]] auto expr() const noexcept -> Node const&;
        };

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
         *      - Among all statements, conditional statement is the only
         *      statement capable of returning a value to its lhs expression.
         *      Other scope-owner statements, except for a few exceptions such as
         *      "defer" and "preface", can achieve value semantics by using the
         *      "yield" statement. Thus, it can be said that "yield" makes the
         *      language a partially expression-oriented language.
         *      - Cannot be placed inside a 1st-level scope of a function.
         *      - Used to exit the current scope and indicate that the value evaluated
         *      by "expr" will be returned and effectively make the statement owning
         *      the exited scope an expression returning the value.
         *      - Unlike "return", "yield" cannot be a scope owner, and it is a syntax
         *      error if "expr" is neither present nor evaluating to a non-void value.
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
         *      - There can only be at most one "preface" statement in a
         *      function's scope, and it must be placed at the beginning of
         *      the scope.
         *      - "preface" accepts one statement, which can be a block statement.
         *      - Like its counter-part, "defer", "preface" is a pure statement
         *      and can be a scope owner.
         */
        struct Preface final : detail::NodeBase {
            Preface(Node stmt, Coords coords);

            [[nodiscard]] auto stmt() const noexcept -> Node const&;
        };

        /**
         * Syntax:
         *      \code
         *          "defer" ("{" stmt* "}" | expr ";")
         *      \endcode
         *
         * Examples:
         *
         * Notes:
         *      - "defer" accepts one statement, which can be a block statement.
         *      - Like its counter-part, "preface", "defer" is a pure statement
         *      and can be a scope owner.
         */
        struct Defer final : detail::NodeBase {
            Defer(Node stmt, Coords coords);

            [[nodiscard]] auto stmt() const noexcept -> Node const&;
        };

        /**
         *
         */
        struct MatchCase final : detail::NodeBase {
            MatchCase(Node value, Node cond, Node stmt, Coords coords);
        };

        /**
         *
         */
        struct Match final : detail::NodeBase {
            Match(Node expr, Vec<Node> cases, Node defaultStmt, Coords coords);
        };


        /**
         *
         */
        struct Loop final : detail::NodeBase {
            Loop(Node decl, Node range, Node body, Coords coords);
        };

        /**
         * Syntax:
         *      \code
         *          expr "=>" stmt ";"
         *      \endcode
         *
         * Examples:
         *      \code
         *          x > 0 => io::println(x);
         *      \endcode
         *      \code
         *          x < 0 => {
         *              ...
         *          }
         *      \endcode
         *      \code
         *          x = y > 0 => y * 2;
         *      \endcode
         *      \code
         *          return x == 0 => y;
         *      \endcode
         *
         * Notes:
         *      - If "stmt" is an expression statement, conditional statement returns
         *      the yields the value of the expression, thus making it the only default
         *      value-semantics statement.
         */
        struct Conditional final : detail::NodeBase {
            Conditional(Node cond, Node then, Coords coords);
        };

        struct Block final : detail::NodeBase {
            Block(Vec<Node> statements, Coords coords);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Assign final : detail::NodeBase {
            Assign(Node lhs, Node rhs, token::EToken op, Coords coords);

            [[nodiscard]] auto op() const noexcept -> token::EToken {
                return m_op;
            }

        private:
            token::EToken m_op;
        };

        struct Expression final : detail::NodeBase {
            Expression(Node expr, Coords coords);
        };
    }

    namespace global {}

    namespace def {}

    struct TranslationUnit final : detail::NodeBase {
        // explicit TranslationUnit(Vec<Node> definitions);
    };
}

#endif // TLC_SYNTAX_NODES_HPP
