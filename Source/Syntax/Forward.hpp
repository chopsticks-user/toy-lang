#ifndef  TOYLANG_SYNTAX_FORWARD_HPP
#define  TOYLANG_SYNTAX_FORWARD_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  class TernaryExpr;
  class BinaryExpr;
  class UnaryExpr;
  class Identifier;
  class FunctionCallExpr;
  class SubScriptingExpr;

  class IntegerLiteral;
  class FloatLiteral;
  class StringLiteral;
  class BooleanLiteral;

  class Program;
  class Function;
  class Block;

  using VNode = std::variant<
    BinaryExpr, Identifier, UnaryExpr, IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral,
    TernaryExpr, FunctionCallExpr, SubScriptingExpr
  >;
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
