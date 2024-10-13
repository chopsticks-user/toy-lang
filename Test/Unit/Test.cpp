#include <catch2/catch_test_macros.hpp>

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"
#include "Syntax/Syntax.hpp"

#include <iostream>

TEST_CASE("Test", "[Test]") {
  using namespace tl::syntax;

  // for (
  //   const tl::lexer::Tokens tokens = tl::util::apply<tl::lexer::Lexer>(std::string(argv[1]));
  //   const auto &token: tokens
  // ) {
  //   std::cout << token.string() << '\n';
  // }

  auto binaryExpr = BinaryExpr(
    FloatLiteral("9.8"),
    UnaryExpr{Identifier("x"), "!"},
    "*"
  );

  auto stringLiteral = StringLiteral(
    "{index}. {bool} statement: the gravitational constant is {val} {unit}",
    {
      IntegerLiteral{"1"},
      BooleanLiteral{"true"},
      binaryExpr.left(),
      StringLiteral{"m/s^2"}
    }
  );

  std::cout << tl::visit<tl::parser::PrettyPrinter>(binaryExpr) << '\n';
  std::cout << tl::visit<tl::parser::PrettyPrinter>(stringLiteral) << '\n';
}
