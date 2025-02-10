#ifndef  TLC_SYNTAX_FORWARD_HPP
#define  TLC_SYNTAX_FORWARD_HPP

#include "core/core.hpp"

namespace tlc::syntax {
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

  using Node = Poly<
    IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral,

    TernaryExpr, BinaryExpr, UnaryExpr, VarId, TypeId, OpId, TupleExpr,
    FunctionCallExpr, SubScriptingExpr, AccessExpr, ArrayExpr,

    ForStmt, ForRangeFragment, MatchStmt, MatchStmtCase, BlockStmt,
    LetStmt, ConditionalStmt, ReturnStmt, AssignStmt, ExprStmt,

    ModuleDecl, ImportDecl, TypeDecl, IdentifierDecl, TupleDecl, GenericDecl,
    FunctionPrototype, FunctionDef, ConceptDef, TranslationUnit,

    TokenNode
  >;

  class NodeBase;

  template<typename T>
  concept IsStrictlyASTNode = std::derived_from<T, NodeBase> && !std::same_as<NodeBase, T>;

  template<typename T>
  concept IsASTNode = IsStrictlyASTNode<T> || std::same_as<Node, T>;

  template<typename T, typename... U>
  concept MatchASTNode = IsStrictlyASTNode<T> &&
                         (IsStrictlyASTNode<U> && ...) &&
                         (std::same_as<T, U> || ...)
                         || std::same_as<Node, T>;

  enum class Storage : u8 {
    Export, Internal, Local,
  };

  enum class Visibility : u8 {
    Public, Private,
  };

  enum class FnType : u8 {
    Function, Method, Field, Operator, Constructor, Prototype,
  };

  const HashSet<Str> overloadableOps = {
    "+", "-", "*", "/",
    "==", "!=",
  };

  const HashSet<Str> assignmentOps = {
    "=", "+=", "-=", "*=", "/=", "%=", "**=",
    "&=", "|=", "^=", "<<=", ">>=",
  };
}

#endif // TLC_SYNTAX_FORWARD_HPP
