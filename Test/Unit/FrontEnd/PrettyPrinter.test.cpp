#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/FrontEnd.hpp"

#include <iostream>

using tl::util::apply;
using tl::fe::Lexer;
using tl::fe::Parser;
using tl::syntax::ASTNode;
using tl::visit;

static std::filesystem::path resourceDir = RESOURCE_DIR;

TEST_CASE("PrettyPrinter: Simple program", "[PrettyPrinter]") {
  // auto tokens = apply<Lexer>(resourceDir / "Simple.toy");
  // auto translationUnit = apply<Parser>(tokens);
  //
  // auto text = visit<PrettyPrinter>(translationUnit);
  // std::cout << text << '\n';

  // auto result = visit<ImportedModuleNameCollector>(translationUnit);
  // for (const auto &name: result) {
  //   std::cout << name << '\n';
  // }
}
