#ifndef  TOYLANG_SYNTAX_FORWARD_HPP
#define  TOYLANG_SYNTAX_FORWARD_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  class TernaryExpr;
  class BinaryExpr;
  class UnaryExpr;
  class Identifier;
  class TypeExpr;
  class FunctionCallExpr;
  class SubScriptingExpr;
  class IdentifierDeclFragment;
  class ParameterDeclFragment;
  class ModuleExpr;

  class IntegerLiteral;
  class FloatLiteral;
  class StringLiteral;
  class BooleanLiteral;

  class BlockStatement;
  class IdentifierDeclStatement;

  class FunctionPrototype;
  class Program;
  class Function;
  class Clazz;
  class TranslationUnit;

  using VNode = std::variant<
    BinaryExpr, Identifier, UnaryExpr, IntegerLiteral, FloatLiteral, StringLiteral,
    BooleanLiteral, TernaryExpr, FunctionCallExpr, SubScriptingExpr, TypeExpr,
    BlockStatement, IdentifierDeclFragment, IdentifierDeclStatement, ParameterDeclFragment,
    FunctionPrototype, Function, ModuleExpr, Clazz, TranslationUnit
  >;
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
