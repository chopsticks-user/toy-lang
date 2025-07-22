#ifndef  TLC_SYNTAX_NODES_HPP
#define  TLC_SYNTAX_NODES_HPP

#include "core/core.hpp"
#include "base.hpp"
#include "token/token.hpp"
#include "util.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::NodeBase {
            explicit Integer(i64 value);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::NodeBase {
            explicit Float(f64 value);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct String final : detail::NodeBase {
            explicit String(
                Str value,
                Vec<Node> placeholders
            );

            [[nodiscard]] auto value() const noexcept -> StrV {
                return m_value;
            }

            // auto placeholder(const sz index) const noexcept -> ASTNode const& {
            //   return childAt(index);
            // }

        private:
            Str m_value;
        };

        struct Boolean final : detail::NodeBase {
            explicit Boolean(bool value);

            [[nodiscard]] auto value() const noexcept -> bool {
                return m_value;
            }

        private:
            bool m_value;
        };

        struct IdentifierBase : detail::NodeBase {
            explicit IdentifierBase(Vec<Str> path = {});

            [[nodiscard]] auto name() const noexcept -> Str {
                return m_path.empty() ? "" : m_path.back();
            }

            [[nodiscard]] auto path() const noexcept -> Str {
                if (m_path.empty()) {
                    return "";
                }

                Str pathStr = m_path.front();

                if (!isImported()) {
                    return pathStr;
                }

                for (StrV s : m_path | rv::drop(1)) {
                    pathStr += "::"s + Str(s);
                }

                return pathStr;
            }

            [[nodiscard]] auto isImported() const noexcept -> bool {
                return m_path.size() > 1;
            }

        protected:
            Vec<Str> m_path;
        };

        struct VarId final : IdentifierBase {
            explicit VarId(Vec<Str> path = {});

            [[nodiscard]] auto isAnonymous() const noexcept -> bool {
                return m_path.empty();
            }
        };

        struct TypeId final : IdentifierBase {
            explicit TypeId(Vec<Str> path);
        };

        struct OpId final : IdentifierBase {
            explicit OpId(Str op);

            [[nodiscard]] auto op() const noexcept -> StrV {
                return m_path.back();
            }
        };

        struct Ternary final : detail::NodeBase {
            Ternary(
                Node operand1,
                Node operand2,
                Node operand3,
                Str op1, Str op2
            );

            [[nodiscard]] auto firstOp() const noexcept -> StrV {
                return m_op1;
            }

            [[nodiscard]] auto secondOp() const noexcept -> StrV {
                return m_op2;
            }

        private:
            Str m_op1;
            Str m_op2;
        };

        struct Binary final : detail::NodeBase {
            Binary(Node lhs, Node rhs, Str op);

            [[nodiscard]] auto op() const noexcept -> StrV {
                return m_op;
            }

        private:
            Str m_op;
        };

        struct Unary final : detail::NodeBase {
            Unary(Node operand, Str op);

            [[nodiscard]] auto op() const noexcept -> StrV {
                return m_op;
            }

        private:
            Str m_op;
        };

        struct Tuple final : detail::NodeBase {
            explicit Tuple(Vec<Node> ids);
        };

        struct FunApp final : detail::NodeBase {
            FunApp(Node callee, Node args);
        };

        struct SubScript final : detail::NodeBase {
            SubScript(Node collection, Node subscript);
        };

        struct Access final : detail::NodeBase {
            Access(Node&& object, Node&& field);
        };

        struct Array final : detail::NodeBase {
            explicit Array(Vec<Node> elements);
        };
    }

    namespace stmt {
        struct ForStmt final : detail::NodeBase {
            ForStmt(Node condition, Node body);
        };

        struct ForRangeFragment final : detail::NodeBase {
            ForRangeFragment(Node iterator, Node iterable);
        };

        struct MatchStmt final : detail::NodeBase {
            MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases);
        };

        struct MatchStmtCase final : detail::NodeBase {
            MatchStmtCase(Node value, Node condition, Node body);
        };

        struct BlockStmt final : detail::NodeBase {
            explicit BlockStmt(Vec<Node> statements);
        };

        struct LetStmt final : detail::NodeBase {
            LetStmt(Node decl, Node init);
        };

        struct ConditionalStmt final : detail::NodeBase {
            ConditionalStmt(Node condition, Node body);
        };

        struct ReturnStmt final : detail::NodeBase {
            explicit ReturnStmt(Node expr);
        };

        struct AssignStmt final : detail::NodeBase {
            AssignStmt(Node left, Node right, Str op);

            [[nodiscard]] auto op() const noexcept -> StrV {
                return m_op;
            }

        private:
            Str m_op;
        };

        struct ExprStmt final : detail::NodeBase {
            explicit ExprStmt(Node expr);
        };
    }

    namespace decl {
        struct Module final : detail::NodeBase {
            explicit Module(Node nsIdentifier, szt line, szt column);
        };

        struct ImportDecl final : detail::NodeBase {
            explicit ImportDecl(Node nsIdentifier);
        };

        // struct TypeDecl final : detail::NodeBase {
        //     explicit TypeDecl(Storage storage, Node identifier, Node typeExpr);
        //
        //     auto storage() const noexcept -> Storage {
        //         return m_storage;
        //     }
        //
        // private:
        //     Storage m_storage;
        // };

        struct IdentifierDecl final : detail::NodeBase {
            IdentifierDecl(bool isMutable, Node const& identifier, Node const& typeExpr);

            auto isMutable() const noexcept -> bool {
                return m_mutable;
            }

        private:
            bool m_mutable;
        };

        struct TupleDecl final : detail::NodeBase {
            explicit TupleDecl(Vec<Node> idDecls);
        };

        struct GenericDecl final : detail::NodeBase {
            explicit GenericDecl(Vec<Node> typeIds);
        };
    }

    // namespace def {
    //     struct FunctionPrototype final : detail::NodeBase {
    //         FunctionPrototype(
    //             FnType fType, Node fIdentifier, Node fParamDecls, Node fReturnDecls
    //         );
    //
    //         auto type() const noexcept -> FnType {
    //             return m_type;
    //         }
    //
    //     private:
    //         FnType m_type;
    //     };
    //
    //     struct FunctionDef final : detail::NodeBase {
    //         FunctionDef(Storage fStorage, Node fPrototype, Node fBody);
    //
    //         auto storage() const noexcept -> Storage {
    //             return m_storage;
    //         }
    //
    //     private:
    //         Storage m_storage;
    //     };
    //
    //     struct ConceptDef final : detail::NodeBase {
    //         ConceptDef(Storage storage, Node identifier, Vec<Node> requirements);
    //
    //         auto storage() const noexcept -> Storage {
    //             return m_storage;
    //         }
    //
    //     private:
    //         Storage m_storage;
    //     };
    // }

    struct TokenWrapper final : detail::NodeBase {
        explicit TokenWrapper(token::Token token);

        auto token() const noexcept -> token::Token {
            return m_token;
        }

    private:
        token::Token m_token;
    };

    // struct TranslationUnit final : detail::NodeBase {
    //     explicit TranslationUnit(Vec<Node> definitions);
    // };
}

#endif // TLC_SYNTAX_NODES_HPP
