#include "driver/driver.hpp"
#include "lex/lex.hpp"
#include "syntax/syntax.hpp"

#include <iostream>

struct PrettyPrinter : tlc::syntax::SyntaxTreeVisitor<tlc::Str> {
  using SyntaxTreeVisitor::operator();

  auto operator()(tlc::syntax::TranslationUnit const &node) -> tlc::Str {
    auto const results = visitChildren(node);
    return "PrettyPrinter: translation unit" + results[0] + results[1] + "\n";
  }

  auto operator()(tlc::syntax::expr::Float const &) -> tlc::Str {
    return " -> float";
  }

  auto operator()(tlc::syntax::expr::Integer const &) -> tlc::Str {
    return " -> integer";
  }
};

struct ValueVisitor : tlc::syntax::SyntaxTreeVisitor<tlc::Str> {
  using SyntaxTreeVisitor::operator();

  auto operator()(tlc::syntax::TranslationUnit const &node) -> tlc::Str {
    auto const results = visitChildren(node);
    return "ValueVisitor: translation unit" + results[0] + results[1] + "\n";
  }

  auto operator()(tlc::syntax::expr::Float const &node) -> tlc::Str {
    return " -> " + std::to_string(node.value());
  }

  auto operator()(tlc::syntax::expr::Integer const &node) -> tlc::Str {
    return " -> " + std::to_string(node.value());
  }
};

struct SideEffect : tlc::syntax::SyntaxTreeVisitor<> {
  using SyntaxTreeVisitor::operator();

  auto operator()(tlc::syntax::TranslationUnit const &node) -> void {
    std::cout << "SideEffect: translation unit";
    visitChildren(node);
    std::cout << '\n';
  }

  auto operator()(tlc::syntax::expr::Float const &) -> void {
    std::cout << " -> float";
  }

  auto operator()(tlc::syntax::expr::Integer const &) -> void {
    std::cout << " -> integer";
  }
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  using tlc::syntax::TranslationUnit;
  using tlc::syntax::expr::Float;
  using tlc::syntax::expr::Integer;
  using tlc::apply;

  std::cout << "tlc\n";
  auto const tu = TranslationUnit{
    {Float{15.0}, Integer{3}}
  };
  std::cout << apply<PrettyPrinter>(tu);
  std::cout << apply<ValueVisitor>(tu);
  apply<SideEffect>(tu);
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

