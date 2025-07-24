#ifndef TLC_SYNTAX_FORWARD_HPP
#define TLC_SYNTAX_FORWARD_HPP

#include "core/core.hpp"

namespace tlc::syntax {
    namespace expr {
        // primary
        struct Integer;
        struct Float;
        struct Boolean;
        struct Identifier;
        // struct String;
        struct Array;
        struct Tuple;

        // postfix
        struct FnApp;
        struct Subscript;
        struct Access;

        struct Prefix;
        struct Binary;
        // struct Ternary;
    }

    // namespace stmt {
    //     struct ForStmt;
    //     struct ForRangeFragment;
    //     struct MatchStmt;
    //     struct MatchStmtCase;
    //     struct BlockStmt;
    //     struct LetStmt;
    //     struct ConditionalStmt;
    //     struct ReturnStmt;
    //     struct AssignStmt;
    //     struct ExprStmt;
    // }
    //
    // namespace decl {
    //     struct Module;
    //     struct ImportDecl;
    //     // struct TypeDecl;
    //     struct GenericDecl;
    //     struct IdentifierDecl;
    //     struct TupleDecl;
    // }
    //
    // // namespace def {
    // //     struct FunctionPrototype;
    // //     struct FunctionDef;
    // //     struct ConceptDef;
    // //     // struct ClassDef;
    // //     // struct EnumDef;
    // //     // struct FlagDef;
    // // }
    //
    // struct TokenWrapper;
    // // struct TranslationUnit;

    using Node = Poly<
        expr::Integer, expr::Float, expr::Boolean, expr::Identifier, expr::Array,
        expr::Tuple, expr::FnApp, expr::Subscript, expr::Access, expr::Prefix,
        expr::Binary
    >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
