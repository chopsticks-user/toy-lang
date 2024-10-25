#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Lexer.hpp"

#include <iostream>
#include <sstream>

using tl::util::apply;
using tl::fe::Lexer;

static std::filesystem::path resourceDir = RESOURCE_DIR;

TEST_CASE("Lexer: Simple program", "[Lexer]") {
  auto tokens = apply<Lexer>(resourceDir / "Simple.toy");
  for (const auto &token: tokens) {
    std::cout << token.string() << '\n';
  }
}

TEST_CASE("Lexer: Another simple program", "[Lexer]") {
  std::istringstream iss;
  iss.str("module foo;\n\n\n\n\n");
  auto tokens = apply<Lexer>(std::move(iss));
  for (const auto &token: tokens) {
    std::cout << token.string() << '\n';
  }
}
