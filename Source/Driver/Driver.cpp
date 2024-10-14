#include "CommandParser.hpp"
#include "ConfigParser.hpp"

#include "FrontEnd/FrontEnd.hpp"
#include "Syntax/Syntax.hpp"

int protected_main(int argc, char **argv) {
  using namespace tl::syntax;

  [[maybe_unused]] auto commandParser = tl::builder::CommandParser{};
  [[maybe_unused]] auto configParser = tl::builder::ConfigParser{};

  for (
    const tl::fe::Tokens tokens = tl::util::apply<tl::fe::Lexer>(std::string(argv[1]));
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

  std::cout << tl::visit<tl::fe::PrettyPrinter>(binaryExpr) << '\n';
  std::cout << tl::visit<tl::fe::PrettyPrinter>(stringLiteral) << '\n';

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
