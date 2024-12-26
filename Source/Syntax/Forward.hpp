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
  class TypeIdentifier;
  class FunctionCallExpr;
  class SubScriptingExpr;
  class AccessExpr;
  class TypeOfExpr;
  class TypeExpr;

  class ModuleDecl;
  class ImportDecl;
  class TypeDecl;
  class IdentifierDecl;
  class TupleDecl;

  // class BlockStatement;
  // class IdentifierDeclStatement;
  // class ReturnStatement;
  // class AssignmentStatement;
  // class MatchStatement;
  // class ForStatement;

  class FunctionDef;
  class MethodDef;
  // class ClassDef;
  // class InterfaceDef;
  // class EnumDef;
  // class FlagDef;

  class TranslationUnit;

  using ASTNode = Poly<
    IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral,

    TernaryExpr, BinaryExpr, UnaryExpr, PostfixUnaryExpr, Identifier,
    FunctionCallExpr, SubScriptingExpr, AccessExpr, TypeOfExpr, TypeExpr,

    // BlockStatement, IdentifierDeclStatement, ReturnStatement,
    // AssignmentStatement, MatchStatement, ForStatement,

    ModuleDecl, ImportDecl, TypeDecl, IdentifierDecl, TupleDecl,
    FunctionDef, MethodDef, TranslationUnit
  >;

  enum class Access : u8 {
    Public, Protected, Private
  };

  enum class Storage : u8 {
    Export, Internal, Local
  };

  enum class Virtuality : u8 {
    Abstract, Final
  };
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
