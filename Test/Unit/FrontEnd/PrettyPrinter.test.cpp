#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/FrontEnd.hpp"

#include <iostream>

using tl::syntax::ASTNode;
using tl::String;
using tl::u64;
using tl::CRef;
using namespace tl::syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(PrettyPrinterTestFixture, __VA_ARGS__)

#define REQUIRE_VTYPE_OF(variant, Type) \
REQUIRE(std::holds_alternative<Type>(variant))

class PrettyPrinterTestFixture {
protected:
  static constexpr auto filepath = "./prettyPrinterTestSample.toy";

  auto print(String source) -> String {
    std::istringstream iss;
    iss.str(std::move(source));
    const ASTNode ast = tl::util::apply<tl::fe::Parser>(
      m_eCollector, filepath, tl::util::apply<tl::fe::Lexer>(std::move(iss))
    );
    m_eCollector.throwAllIfExists();
    return tl::util::apply<tl::fe::PrettyPrinter>(astCast<TranslationUnit>(ast));
  }

private:
  tl::ExpceptionCollector m_eCollector;
};

TEST_CASE_WITH_FIXTURE("PrettyPrinter: module, import and type declarations", "[PrettyPrinter]") {
  String source = R"(
module    foo;
import std  ::io ;
import  std:: math;
)";

  String expected = R"(
module foo;

import std::io;
import std::math;
)";

  REQUIRE(print(std::move(source)) == expected);
}
