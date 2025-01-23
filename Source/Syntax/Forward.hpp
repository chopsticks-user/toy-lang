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
  class VarId;
  class TypeId;
  class OpId;
  class TupleExpr;
  class FunctionCallExpr;
  class SubScriptingExpr;
  class AccessExpr;
  class ArrayExpr;

  class ModuleDecl;
  class ImportDecl;
  class TypeDecl;
  class GenericDecl;
  class IdentifierDecl;
  class TupleDecl;

  class ForStmt;
  class ForRangeFragment;
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

  class TokenNode;

  using ASTNode = Poly<
    IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral,

    TernaryExpr, BinaryExpr, UnaryExpr, VarId, TypeId, OpId, TupleExpr,
    FunctionCallExpr, SubScriptingExpr, AccessExpr, ArrayExpr,

    ForStmt, ForRangeFragment, MatchStmt, MatchStmtCase, BlockStmt,
    LetStmt, ConditionalStmt, ReturnStmt, AssignStmt, ExprStmt,

    ModuleDecl, ImportDecl, TypeDecl, IdentifierDecl, TupleDecl, GenericDecl,
    FunctionPrototype, FunctionDef, ConceptDef, TranslationUnit,

    TokenNode
  >;

  class ASTNodeBase;

  template<typename T>
  concept IsStrictlyASTNode = std::derived_from<T, ASTNodeBase> && !std::same_as<ASTNodeBase, T>;

  template<typename T>
  concept IsASTNode = IsStrictlyASTNode<T> || std::same_as<ASTNode, T>;

  template<typename T, typename... U>
  concept MatchASTNode = IsStrictlyASTNode<T> &&
                         (IsStrictlyASTNode<U> && ...) &&
                         (std::same_as<T, U> || ...)
                         || std::same_as<ASTNode, T>;

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
