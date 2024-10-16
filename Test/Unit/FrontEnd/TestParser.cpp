#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

#include <iostream>

using tl::util::apply;
using tl::fe::Lexer;
using tl::fe::Parser;
using tl::syntax::VNode;

static std::filesystem::path resourceDir = RESOURCE_DIR;

TEST_CASE("Parser: Simple program", "[Parser]") {
  auto tokens = apply<Lexer>(resourceDir / "Simple.toy");
  auto translationUnit = apply<Parser>(tokens);
}
