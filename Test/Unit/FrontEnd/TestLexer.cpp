#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Lexer.hpp"

#include <iostream>

using tl::util::apply;
using tl::fe::Lexer;

std::filesystem::path resourceDir = RESOURCE_DIR;

TEST_CASE("Test lexer", "[Lexer]") {
  // todo: fix operator lexing
  auto tokens = apply<Lexer>(resourceDir / "test1.toy");
  for (const auto &token: tokens) {
    std::cout << token.string() << '\n';
  }
}
