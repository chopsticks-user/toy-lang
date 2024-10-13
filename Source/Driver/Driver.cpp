#include "CommandParser.hpp"
#include "ConfigParser.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"
#include "Syntax/Syntax.hpp"

int protected_main(int argc, char **argv) {
  using namespace tl::syntax;

  [[maybe_unused]] auto commandParser = tl::builder::CommandParser{};
  [[maybe_unused]] auto configParser = tl::builder::ConfigParser{};

  for (
    const tl::lexer::Tokens tokens = tl::util::apply<tl::lexer::Lexer>(std::string(argv[1]));
    const auto &token: tokens
  ) {
    std::cout << token.string() << '\n';
  }

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

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
  } catch (...) {
    std::cerr << "Uncaught exception" << '\n';
  }
  return EXIT_FAILURE;
}
