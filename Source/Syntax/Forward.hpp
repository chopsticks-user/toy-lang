#ifndef  TOYLANG_SYNTAX_FORWARD_HPP
#define  TOYLANG_SYNTAX_FORWARD_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  class IntegerLiteral;
  class FloatLiteral;
  class StringLiteral;
  class BooleanLiteral;

  class TernaryExpr;
  class BinaryExpr;
  class UnaryExpr;
  class PostfixUnaryExpr;
  class Identifier;
  class TupleExpr;
  class FunctionCallExpr;
  class SubScriptingExpr;
  class AccessExpr;
  class TypeOfExpr;
  class TypeExpr;
  class ArrayExpr;

  class ModuleDecl;
  class ImportDecl;
  class TypeDecl;
  class IdentifierDecl;
  class TupleDecl;

  class ForStmt;
  class MatchStmt;
  class MatchStmtCase;
  class BlockStmt;
  class LetStmt;
  class ConditionalStmt;
  class ReturnStmt;
  class AssignStmt;
  class ExprStmt;

  class FunctionPrototype;
  class FunctionDef;
  class ConceptDef;
  // class ClassDef;
  // class EnumDef;
  // class FlagDef;

  class TranslationUnit;

  using ASTNode = Poly<
    IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral,

    TernaryExpr, BinaryExpr, UnaryExpr, PostfixUnaryExpr, Identifier, TupleExpr,
    FunctionCallExpr, SubScriptingExpr, AccessExpr, TypeOfExpr, TypeExpr, ArrayExpr,

    ForStmt, MatchStmt, MatchStmtCase, BlockStmt,
    LetStmt, ConditionalStmt, ReturnStmt, AssignStmt, ExprStmt,

    ModuleDecl, ImportDecl, TypeDecl, IdentifierDecl, TupleDecl,
    FunctionPrototype, FunctionDef, ConceptDef, TranslationUnit
  >;

  enum class Storage : u8 {
    Export, Internal, Local,
  };

  enum class Visibility : u8 {
    Public, Private,
  };

  enum class FnType : u8 {
    Function, Method, Field, Operator, Constructor, Prototype,
  };

  const HashSet<String> overloadableOps = {
    "+", "-", "*", "/",
    "==", "!=",
  };

  const HashSet<String> assignmentOps = {
    "=", "+=", "-=", "*=", "/=", "%=", "**=",
    "&=", "|=", "^=", "<<=", ">>=",
  };
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
