#include "CommandParser.hpp"
#include "ConfigParser.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

int protected_main(int argc, char **argv) {
  [[maybe_unused]] auto commandParser = tl::builder::CommandParser{};
  [[maybe_unused]] auto configParser = tl::builder::ConfigParser{};

  // tl::lexer::Tokens tokens = tl::util::apply<tl::lexer::Lexer>(std::string(argv[1]));
  //
  // for (const auto &token: tokens) {
  //   std::cout << token.string() << '\n';
  // }

  tl::parser::ast::VNode binaryExpr = tl::parser::ast::BinaryExpr{
    std::make_unique<tl::parser::ast::VNode>(
      tl::parser::ast::Number("9.9")
    ),
    std::make_unique<tl::parser::ast::VNode>(
      tl::parser::ast::UnaryExpr(
        std::make_unique<tl::parser::ast::VNode>(
          tl::parser::ast::Number("2.5")
        ), "-")
    ),
    "*"
  };

  std::cout << std::visit(tl::parser::PrettyPrinter{}, binaryExpr) << '\n';

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
