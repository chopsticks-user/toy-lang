#ifndef TLC_SYNTAX_FORWARD_HPP
#define TLC_SYNTAX_FORWARD_HPP

#include "core/core.hpp"

namespace tlc::syntax {
    namespace expr {
        struct Integer;
        struct Float;
        struct Boolean;

        struct VarId;
        struct TypeId;

        // struct String;
        //

        // struct OpId;
        //
        // struct Ternary;
        // struct Binary;
        // struct Unary;
        // struct Tuple;
        // struct FunApp;
        // struct SubScript;
        // struct Access;
        // struct Array;
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
        expr::Integer, expr::Float, expr::Boolean,
        expr::VarId, expr::TypeId
    >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
