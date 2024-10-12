#ifndef  TOYLANG_SYNTAX_FORWARD_HPP
#define  TOYLANG_SYNTAX_FORWARD_HPP

#include "Core/Core.hpp"

namespace tl::parser::syntax {
  class Program;
  class Function;
  class BinaryExpr;
  class UnaryExpr;
  class Block;
  class Number;
  class Identifier;
  class StringLiteral;

  using VNode = std::variant<BinaryExpr, Number, Identifier, UnaryExpr, StringLiteral>;
  using VExpr = std::variant<BinaryExpr, Number, Identifier, UnaryExpr, StringLiteral>;
}

#endif // TOYLANG_SYNTAX_FORWARD_HPP
