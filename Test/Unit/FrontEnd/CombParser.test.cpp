#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <utility>

#include "FrontEnd/CombParser.hpp"

using tl::String;
using tl::sz;
using tl::CRef;
using tl::Opt;
using tl::Vec;
using tl::Strings;
using namespace tl::syntax;
using namespace tl::fe;

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(CombinatoryParserTestFixture, __VA_ARGS__)

class CombinatoryParserTestFixture {
protected:
  static constexpr auto filepath = "./combinatoryParserTestSample.toy";

  template<typename T = ASTNode>
    requires (std::derived_from<T, ASTNodeBase> || std::same_as<T, ASTNode>)
  static auto parse(String source, CRef<UnitParser> parser) -> std::tuple<T, Strings, CParsingDriver> {
    std::istringstream iss;
    iss.str(std::move(source));
    auto driver = CParsingDriver{
      filepath, tl::apply<Lexer>(std::move(iss))
    };

    auto [nodes, errors] = parser(driver);
    if constexpr (std::same_as<T, ASTNode>) {
      return {std::move(nodes), std::move(errors), std::move(driver)};
    } else {
      return {astCast<T>(nodes.front()), std::move(errors), std::move(driver)};
    }
  }
};

namespace expression {
  TEST_CASE_WITH_FIXTURE("Combinatory Parser: variable identifier", "[CombinatoryParser]") {
    SECTION("Anonymous") {
      auto [id, errors, driver] = parse<Identifier>(
        R"(_ )", variableIdExpr()
      );
      REQUIRE(driver.done());
      REQUIRE(errors.empty());

      REQUIRE(id.name().empty());
      REQUIRE(id.path().empty());
      REQUIRE(id.isAnonymous());
      REQUIRE_FALSE(id.isImported());
      REQUIRE_FALSE(id.isType());
      REQUIRE_FALSE(id.isOverloadedOp());
    }

    SECTION("Imported") {
      auto [id, errors, driver] = parse<Identifier>(
        R"(foo::bar::Int )", variableIdExpr()
      );
      REQUIRE(driver.done());
      REQUIRE(errors.empty());

      REQUIRE(id.name() == "Int");
      REQUIRE(id.path() == "foo::bar::Int");
      REQUIRE_FALSE(id.isAnonymous());
      REQUIRE(id.isImported());
      REQUIRE(id.isType());
      REQUIRE_FALSE(id.isOverloadedOp());
    }
  }

  TEST_CASE_WITH_FIXTURE("Combinatory Parser: type identifier", "[CombinatoryParser]") {
    SECTION("Anonymous") {
      auto [id, errors, driver] = parse<Identifier>(
        R"(_ )", variableIdExpr()
      );
      REQUIRE(driver.done());
      REQUIRE(errors.empty());

      REQUIRE(id.name().empty());
      REQUIRE(id.path().empty());
      REQUIRE(id.isAnonymous());
      REQUIRE_FALSE(id.isImported());
      REQUIRE_FALSE(id.isType());
      REQUIRE_FALSE(id.isOverloadedOp());
    }

    SECTION("Imported") {
      auto [id, errors, driver] = parse<Identifier>(
        R"(foo::bar::Int )", variableIdExpr()
      );
      REQUIRE(driver.done());
      REQUIRE(errors.empty());

      REQUIRE(id.name() == "Int");
      REQUIRE(id.path() == "foo::bar::Int");
      REQUIRE_FALSE(id.isAnonymous());
      REQUIRE(id.isImported());
      REQUIRE(id.isType());
      REQUIRE_FALSE(id.isOverloadedOp());
    }
  }
}

namespace statement {
}

namespace definition {
}
