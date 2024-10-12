#include "CommandParser.hpp"
#include "ConfigParser.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

int protected_main(int argc, char **argv) {
  using namespace tl::parser::ast;

  [[maybe_unused]] auto commandParser = tl::builder::CommandParser{};
  [[maybe_unused]] auto configParser = tl::builder::ConfigParser{};

  tl::lexer::Tokens tokens = tl::util::apply<tl::lexer::Lexer>(std::string(argv[1]));

  for (const auto &token: tokens) {
    std::cout << token.string() << '\n';
  }

  auto binaryExpr = BinaryExpr(
    Number("9.8"),
    UnaryExpr{Identifier("x"), "!"},
    "*"
  );

  auto stringLiteral = StringLiteral(
    "Gravitational constant: {val} {unit}",
    {binaryExpr.left(), StringLiteral{"m/s^2"}}
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
