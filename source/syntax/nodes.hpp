#ifndef  TLC_SYNTAX_NODES_HPP
#define  TLC_SYNTAX_NODES_HPP

#include "core/core.hpp"
#include "base.hpp"
#include "token/token.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::NodeBase {
            Integer(i64 value, token::Token::Coords coords);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::NodeBase {
            Float(f64 value, token::Token::Coords coords);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct Boolean final : detail::NodeBase {
            Boolean(b8 value, token::Token::Coords coords);

            [[nodiscard]] auto value() const noexcept -> b8 {
                return m_value;
            }

        private:
            b8 m_value;
        };

        struct Identifier : detail::NodeBase {
            Identifier(Vec<Str> path, token::EToken type, token::Token::Coords coords);

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

            [[nodiscard]] auto type() const noexcept -> token::EToken {
                return m_type;
            }

            [[nodiscard]] auto isImported() const noexcept -> bool {
                return m_path.size() > 1;
            }

        private:
            Vec<Str> m_path;
            token::EToken m_type;
        };

        struct Array final : detail::NodeBase {
            Array(Vec<Node> elements, token::Token::Coords coords);
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> elements, token::Token::Coords coords);
        };

        struct FnApp final : detail::NodeBase {
            FnApp(Node callee, Node args, token::Token::Coords coords);
        };

        struct Subscript final : detail::NodeBase {
            Subscript(
                Node collection, Node subscript, token::Token::Coords coords
            );
        };

        struct Access final : detail::NodeBase {
            Access(Node object, Node field, token::Token::Coords coords);
        };

        struct Unary final : detail::NodeBase {
            Unary(
                Node operand, token::EToken op, token::Token::Coords coords
            );

            [[nodiscard]] auto op() const noexcept -> token::EToken {
                return m_op;
            }

            [[nodiscard]] auto operand() const noexcept -> Node;

        private:
            token::EToken m_op;
        };

        struct Binary final : detail::NodeBase {
            Binary(
                Node lhs, Node rhs, token::EToken op, token::Token::Coords coords
            );

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
        //
        //
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
        //
        //
    }

    // namespace stmt {
    //     struct ForStmt final : detail::NodeBase {
    //         ForStmt(Node condition, Node body);
    //     };
    //
    //     struct ForRangeFragment final : detail::NodeBase {
    //         ForRangeFragment(Node iterator, Node iterable);
    //     };
    //
    //     struct MatchStmt final : detail::NodeBase {
    //         MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases);
    //     };
    //
    //     struct MatchStmtCase final : detail::NodeBase {
    //         MatchStmtCase(Node value, Node condition, Node body);
    //     };
    //
    //     struct BlockStmt final : detail::NodeBase {
    //         explicit BlockStmt(Vec<Node> statements);
    //     };
    //
    //     struct LetStmt final : detail::NodeBase {
    //         LetStmt(Node decl, Node init);
    //     };
    //
    //     struct ConditionalStmt final : detail::NodeBase {
    //         ConditionalStmt(Node condition, Node body);
    //     };
    //
    //     struct ReturnStmt final : detail::NodeBase {
    //         explicit ReturnStmt(Node expr);
    //     };
    //
    //     struct AssignStmt final : detail::NodeBase {
    //         AssignStmt(Node left, Node right, Str op);
    //
    //         [[nodiscard]] auto op() const noexcept -> StrV {
    //             return m_op;
    //         }
    //
    //     private:
    //         Str m_op;
    //     };
    //
    //     struct ExprStmt final : detail::NodeBase {
    //         explicit ExprStmt(Node expr);
    //     };
    // }
    //
    // namespace decl {
    //     struct Module final : detail::NodeBase {
    //         explicit Module(Node nsIdentifier, szt line, szt column);
    //     };
    //
    //     struct ImportDecl final : detail::NodeBase {
    //         explicit ImportDecl(Node nsIdentifier);
    //     };
    //
    //     // struct TypeDecl final : detail::NodeBase {
    //     //     explicit TypeDecl(Storage storage, Node identifier, Node typeExpr);
    //     //
    //     //     auto storage() const noexcept -> Storage {
    //     //         return m_storage;
    //     //     }
    //     //
    //     // private:
    //     //     Storage m_storage;
    //     // };
    //
    //     struct IdentifierDecl final : detail::NodeBase {
    //         IdentifierDecl(bool isMutable, Node const& identifier, Node const& typeExpr);
    //
    //         auto isMutable() const noexcept -> bool {
    //             return m_mutable;
    //         }
    //
    //     private:
    //         bool m_mutable;
    //     };
    //
    //     struct TupleDecl final : detail::NodeBase {
    //         explicit TupleDecl(Vec<Node> idDecls);
    //     };
    //
    //     struct GenericDecl final : detail::NodeBase {
    //         explicit GenericDecl(Vec<Node> typeIds);
    //     };
    // }
    //
    // // namespace def {
    // //     struct FunctionPrototype final : detail::NodeBase {
    // //         FunctionPrototype(
    // //             FnType fType, Node fIdentifier, Node fParamDecls, Node fReturnDecls
    // //         );
    // //
    // //         auto type() const noexcept -> FnType {
    // //             return m_type;
    // //         }
    // //
    // //     private:
    // //         FnType m_type;
    // //     };
    // //
    // //     struct FunctionDef final : detail::NodeBase {
    // //         FunctionDef(Storage fStorage, Node fPrototype, Node fBody);
    // //
    // //         auto storage() const noexcept -> Storage {
    // //             return m_storage;
    // //         }
    // //
    // //     private:
    // //         Storage m_storage;
    // //     };
    // //
    // //     struct ConceptDef final : detail::NodeBase {
    // //         ConceptDef(Storage storage, Node identifier, Vec<Node> requirements);
    // //
    // //         auto storage() const noexcept -> Storage {
    // //             return m_storage;
    // //         }
    // //
    // //     private:
    // //         Storage m_storage;
    // //     };
    // // }
    //
    // struct TokenWrapper final : detail::NodeBase {
    //     explicit TokenWrapper(token::Token token);
    //
    //     auto token() const noexcept -> token::Token {
    //         return m_token;
    //     }
    //
    // private:
    //     token::Token m_token;
    // };
    //
    // // struct TranslationUnit final : detail::NodeBase {
    // //     explicit TranslationUnit(Vec<Node> definitions);
    // // };
}

#endif // TLC_SYNTAX_NODES_HPP
