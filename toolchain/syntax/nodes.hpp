#ifndef  TLC_SYNTAX_NODES_HPP
#define  TLC_SYNTAX_NODES_HPP

#include "core/core.hpp"
#include "base.hpp"
#include "token/token.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::TerminalNodeBase {
            Integer(i64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::TerminalNodeBase {
            Float(f64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct Boolean final : detail::TerminalNodeBase {
            Boolean(b8 value, Location location);

            [[nodiscard]] auto value() const noexcept -> b8 {
                return m_value;
            }

        private:
            b8 m_value;
        };

        struct Identifier : detail::TerminalNodeBase, detail::IdentifierBase {
            Identifier(Vec<Str> path, Location location);
        };

        struct Array final : detail::InternalNodeBase {
            Array(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Tuple final : detail::InternalNodeBase {
            Tuple(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct FnApp final : detail::InternalNodeBase {
            FnApp(Node callee, Node args, Location location);
        };

        struct Subscript final : detail::InternalNodeBase {
            Subscript(Node collection, Node subscript, Location location);
        };

        struct Prefix final : detail::InternalNodeBase {
            Prefix(Node operand, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Binary final : detail::InternalNodeBase {
            Binary(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct String final : detail::InternalNodeBase {
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

        struct RecordEntry final : detail::InternalNodeBase {
            RecordEntry(Str key, Node value, Location location);

            [[nodiscard]] auto key() const noexcept -> StrV {
                return m_key;
            }

        private:
            Str m_key;
        };

        struct Record final : detail::InternalNodeBase {
            Record(Node type, Vec<Node> entries, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Try final : detail::InternalNodeBase {
            Try(Node expr, Location location);
        };
    }

    namespace type {
        struct Identifier : detail::TerminalNodeBase, detail::IdentifierBase {
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

        struct Array final : detail::InternalNodeBase {
            Array(Node type, Node sizes, Location location);
        };

        struct Tuple final : detail::InternalNodeBase {
            Tuple(Vec<Node> types, Location location);

            [[nodiscard]] auto size() const -> szt;
        };

        struct Function final : detail::InternalNodeBase {
            Function(Node args, Node result, Location location);
        };

        struct Infer final : detail::InternalNodeBase {
            Infer(Node expr, Location location);

            [[nodiscard]] auto expr() const noexcept -> Node;
        };

        struct GenericArguments final : detail::InternalNodeBase {
            GenericArguments(Vec<Node> args, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Generic final : detail::InternalNodeBase {
            Generic(Node type, Node args, Location location);
        };

        struct Binary final : detail::InternalNodeBase {
            Binary(Node lhs, lexeme::Lexeme op, Node rhs, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };
    }

    namespace decl {
        struct Identifier final : detail::InternalNodeBase {
            Identifier(Str name, Node type, Location location);

            [[nodiscard]] auto name() const noexcept -> Str const& {
                return m_name;
            }

            [[nodiscard]] auto inferred() const noexcept -> b8;

        private:
            Str m_name;
        };

        struct Tuple final : detail::InternalNodeBase {
            Tuple(Vec<Node> decls, Location location);

            [[nodiscard]] auto decl(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };

        struct GenericIdentifier final : detail::TerminalNodeBase {
            GenericIdentifier(Str name, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct GenericParameters final : detail::InternalNodeBase {
            GenericParameters(Vec<Node> params, Location location);

            [[nodiscard]] auto size() const -> szt;
        };
    }

    namespace stmt {
        struct Decl final : detail::InternalNodeBase {
            Decl(Node decl, Node initializer, Location location);

            [[nodiscard]] auto defaultInitialized() const -> bool;
        };

        struct Return final : detail::InternalNodeBase {
            Return(Node expr, Location location);
        };

        struct Defer final : detail::InternalNodeBase {
            Defer(Node stmt, Location location);
        };

        struct MatchCase final : detail::InternalNodeBase {
            MatchCase(Node value, Node cond, Node stmt, Location location);
        };

        struct Match final : detail::InternalNodeBase {
            Match(Node expr, Vec<Node> cases, Node defaultStmt, Location location);
        };


        struct Loop final : detail::InternalNodeBase {
            Loop(Node decl, Node range, Node body, Location location);
        };

        struct Conditional final : detail::InternalNodeBase {
            Conditional(Node cond, Node then, Location location);
        };

        struct Block final : detail::InternalNodeBase {
            Block(Vec<Node> statements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Assign final : detail::InternalNodeBase {
            Assign(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Expression final : detail::InternalNodeBase {
            Expression(Node expr, Location location);
        };
    }

    namespace global {
        struct ModuleDecl final : detail::InternalNodeBase {
            ModuleDecl(Node path, Location location);
        };

        struct ImportDeclGroup final : detail::InternalNodeBase {
            ImportDeclGroup(Vec<Node> imports, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct ImportDecl final : detail::InternalNodeBase {
            ImportDecl(Node alias, Node path, Location location);
        };

        struct FunctionPrototype final : detail::InternalNodeBase {
            FunctionPrototype(Node genericDecl, Str name, Node paramsDecl,
                              Node returnsDecl, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct Function final : detail::InternalNodeBase {
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

    struct RequiredButMissing final {
        constexpr RequiredButMissing() = default;
    };

    using Empty = std::monostate;

    struct TranslationUnit final : detail::InternalNodeBase {
        TranslationUnit(
            fs::path sourcePath, Node moduleDecl,
            Node importDeclGroup, Vec<Node> definitions
        );

        [[nodiscard]] auto sourcePath() const noexcept -> fs::path const& {
            return m_sourcePath;
        }

        [[nodiscard]] auto size() const noexcept -> szt;

    private:
        fs::path m_sourcePath;
    };
}

#endif // TLC_SYNTAX_NODES_HPP
