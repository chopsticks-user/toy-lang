#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/FrontEnd.hpp"

#include <iostream>

using tl::String;
using namespace tl::syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
  TEST_CASE_METHOD(PrettyPrinterTestFixture, __VA_ARGS__)

#define REQUIRE_PRETTY_PRINTER(source, expected) \
  REQUIRE(print(std::move(source)) == expected)

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

export type   Number = math ::Complex   | Int| Float  ;
type Complex=math::Complex;
)";

  String expected = R"(
module foo;

import std::io;
import std::math;

export type Number = math::Complex | Int | Float;

internal type Complex = math::Complex;
)";

  REQUIRE_PRETTY_PRINTER(source, expected);
}
