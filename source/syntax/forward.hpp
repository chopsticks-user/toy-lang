#ifndef  TLC_SYNTAX_FORWARD_HPP
#define  TLC_SYNTAX_FORWARD_HPP

#include "core/core.hpp"

namespace tlc::syntax {
  namespace expr {
    struct TokenNode;

    struct Integer;
    struct Float;
    struct String;
    struct Boolean;

    struct VarId;
    struct TypeId;
    struct OpId;

    struct Ternary;
    struct Binary;
    struct Unary;
    struct Tuple;
    struct FunApp;
    struct SubScript;
    struct Access;
    struct Array;
  }

  namespace stmt {
    struct ForStmt;
    struct ForRangeFragment;
    struct MatchStmt;
    struct MatchStmtCase;
    struct BlockStmt;
    struct LetStmt;
    struct ConditionalStmt;
    struct ReturnStmt;
    struct AssignStmt;
    struct ExprStmt;
  }

  namespace decl {
    struct ModuleDecl;
    struct ImportDecl;
    struct TypeDecl;
    struct GenericDecl;
    struct IdentifierDecl;
    struct TupleDecl;
  }

  namespace def {
    struct FunctionPrototype;
    struct FunctionDef;
    struct ConceptDef;
    // struct ClassDef;
    // struct EnumDef;
    // struct FlagDef;
  }

  struct TranslationUnit;

  using Node = Poly<
    expr::TokenNode,
    expr::Integer, expr::Float, expr::String, expr::Boolean,
    expr::VarId, expr::TypeId, expr::OpId,
    expr::Ternary, expr::Binary, expr::Unary, expr::Tuple,
    expr::FunApp, expr::SubScript, expr::Access, expr::Array,

    stmt::ForStmt, stmt::ForRangeFragment, stmt::MatchStmt, stmt::MatchStmtCase,
    stmt::BlockStmt, stmt::LetStmt, stmt::ConditionalStmt, stmt::ReturnStmt,
    stmt::AssignStmt, stmt::ExprStmt,

    // decl::ModuleDecl, decl::ImportDecl, decl::TypeDecl,
    // decl::IdentifierDecl, decl::TupleDecl, decl::GenericDecl,
    // def::FunctionPrototype, def::FunctionDef, def::ConceptDef,

    TranslationUnit
  >;
}

#endif // TLC_SYNTAX_FORWARD_HPP
