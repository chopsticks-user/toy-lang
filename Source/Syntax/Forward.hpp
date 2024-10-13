#ifndef  TOYLANG_SYNTAX_FORWARD_HPP
#define  TOYLANG_SYNTAX_FORWARD_HPP

#include "Core/Core.hpp"

namespace tl::syntax {
  class BinaryExpr;
  class UnaryExpr;
  class Identifier;

  class IntegerLiteral;
  class FloatLiteral;
  class StringLiteral;
  class BooleanLiteral;

  class Program;
  class Function;
  class Block;

  using VNode = std::variant<
    BinaryExpr, Identifier, UnaryExpr, IntegerLiteral, FloatLiteral, StringLiteral, BooleanLiteral
  >;
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
