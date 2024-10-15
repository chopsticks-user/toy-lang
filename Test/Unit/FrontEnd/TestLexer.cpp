#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Lexer.hpp"

#include <iostream>

using tl::util::apply;
using tl::fe::Lexer;

std::filesystem::path resourceDir = RESOURCE_DIR;

TEST_CASE("Simple program", "[Lexer]") {
  auto tokens = apply<Lexer>(resourceDir / "Simple.toy");
  for (const auto &token: tokens) {
    std::cout << token.string() << '\n';
  }
}
