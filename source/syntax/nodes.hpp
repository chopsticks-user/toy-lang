#ifndef  TLC_SYNTAX_NODES_HPP
#define  TLC_SYNTAX_NODES_HPP

#include "core/core.hpp"
#include "base.hpp"
#include "token/token.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::NodeBase {
            Integer(i64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::NodeBase {
            Float(f64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct Boolean final : detail::NodeBase {
            Boolean(b8 value, Location location);

            [[nodiscard]] auto value() const noexcept -> b8 {
                return m_value;
            }

        private:
            b8 m_value;
        };

        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(Vec<Str> path, Location location);
        };

        struct Array final : detail::NodeBase {
            Array(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct FnApp final : detail::NodeBase {
            FnApp(Node callee, Node args, Location location);
        };

        struct Subscript final : detail::NodeBase {
            Subscript(Node collection, Node subscript, Location location);
        };

        struct Prefix final : detail::NodeBase {
            Prefix(Node operand, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Binary final : detail::NodeBase {
            Binary(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

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

        struct String final : detail::NodeBase {
            String(Vec<Str> fragments, Vec<Node> placeholders, Location location);

            [[nodiscard]] auto fragments() const noexcept -> Span<Str const> {
                return m_fragments;
            }

            [[nodiscard]] auto nPlaceholders() const noexcept -> szt {
                return m_fragments.size() - 1;
            }

            [[nodiscard]] auto interpolated() const noexcept -> b8 {
                return nPlaceholders() > 0;
            }

        private:
            Vec<Str> m_fragments;
        };

        struct RecordEntry final : detail::NodeBase {
            RecordEntry(Str key, Node value, Location location);

            [[nodiscard]] auto key() const noexcept -> StrV {
                return m_key;
            }

        private:
            Str m_key;
        };

        struct Record final : detail::NodeBase {
            Record(Node type, Vec<Node> entries, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Try final : detail::NodeBase {
            Try(Node expr, Location location);
        };
    }

    namespace type {
        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(
                b8 constant, Vec<Str> path, b8 fundamental, Location location
            );

            [[nodiscard]] auto fundamental() const noexcept -> bool {
                return m_fundamental;
            }

            [[nodiscard]] auto constant() const noexcept -> bool {
                return m_constant;
            }

        private:
            b8 m_fundamental;
            b8 m_constant;
        };

        struct Array final : detail::NodeBase {
            Array(Node type, Vec<Node> sizes, Location location);

            [[nodiscard]] auto nDims() const noexcept -> szt;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> types, Location location);

            [[nodiscard]] auto size() const -> szt;
        };

        struct Function final : detail::NodeBase {
            Function(Node args, Node result, Location location);
        };

        /**
         * [[expr]]
         */
        struct Infer final : detail::NodeBase {
            Infer(Node expr, Location location);

            [[nodiscard]] auto expr() const noexcept -> Node;
        };

        struct GenericArguments final : detail::NodeBase {
            GenericArguments(Vec<Node> args, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Generic final : detail::NodeBase {
            Generic(Node type, Node args, Location location);
        };
    }

    namespace decl {
        struct Identifier final : detail::NodeBase {
            Identifier(Str name, Node type, Location location);

            [[nodiscard]] auto name() const noexcept -> Str const& {
                return m_name;
            }

            [[nodiscard]] auto inferred() const noexcept -> b8;

        private:
            Str m_name;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> decls, Location location);

            [[nodiscard]] auto decl(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };

        struct GenericIdentifier final : detail::NodeBase {
            GenericIdentifier(Str name, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct GenericParameters final : detail::NodeBase {
            GenericParameters(Vec<Node> params, Location location);

            [[nodiscard]] auto size() const -> szt;
        };
    }

    namespace stmt {
        struct Decl final : detail::NodeBase {
            Decl(Node decl, Node initializer, Location location);

            [[nodiscard]] auto defaultInitialized() const -> bool;
        };

        struct Return final : detail::NodeBase {
            Return(Node expr, Location location);
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
            Defer(Node stmt, Location location);
        };

        /**
         *
         */
        struct MatchCase final : detail::NodeBase {
            MatchCase(Node value, Node cond, Node stmt, Location location);
        };

        /**
         *
         */
        struct Match final : detail::NodeBase {
            Match(Node expr, Vec<Node> cases, Node defaultStmt, Location location);
        };


        /**
         *
         */
        struct Loop final : detail::NodeBase {
            Loop(Node decl, Node range, Node body, Location location);
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
            Conditional(Node cond, Node then, Location location);
        };

        struct Block final : detail::NodeBase {
            Block(Vec<Node> statements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Assign final : detail::NodeBase {
            Assign(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Expression final : detail::NodeBase {
            Expression(Node expr, Location location);
        };
    }

    namespace global {
        struct ModuleDecl final : detail::NodeBase {
            ModuleDecl(Node path, Location location);
        };

        struct ImportDecl final : detail::NodeBase {
            ImportDecl(Node alias, Node path, Location location);
        };

        struct FunctionPrototype final : detail::NodeBase {
            FunctionPrototype(Node genericDecl, Str name, Node paramsDecl,
                              Node returnsDecl, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct Function final : detail::NodeBase {
            Function(lexeme::Lexeme visibility, Node prototype, Node body,
                     Location location);

            [[nodiscard]] auto visibility() const noexcept
                -> lexeme::Lexeme const& {
                return m_visibility;
            }

        private:
            lexeme::Lexeme m_visibility;
        };
    }

    struct TranslationUnit final : detail::NodeBase {
        TranslationUnit(fs::path sourcePath, Vec<Node> definitions);

        [[nodiscard]] auto sourcePath() const noexcept -> fs::path const& {
            return m_sourcePath;
        }

        [[nodiscard]] auto size() const noexcept -> szt;

    private:
        fs::path m_sourcePath;
    };
}

#endif // TLC_SYNTAX_NODES_HPP
