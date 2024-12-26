#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

using tl::String;
using tl::u64;
using tl::CRef;
using namespace tl::syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(ParserTestFixture, __VA_ARGS__)

#define REQUIRE_VTYPE_OF(variant, Type) \
  REQUIRE(std::holds_alternative<Type>(variant))

class ParserTestFixture {
protected:
  static constexpr auto filepath = "./parserTestSample.toy";

  auto parse(String source) -> void {
    std::istringstream iss;
    iss.str(std::move(source));
    m_ast = tl::util::apply<tl::fe::Parser>(
      m_eCollector, filepath, tl::util::apply<tl::fe::Lexer>(std::move(iss))
    );
    m_eCollector.throwAllIfExists();
    REQUIRE_VTYPE_OF(m_ast, TranslationUnit);
  }

  template<std::derived_from<ASTNodeBase> TNode>
  auto nodeAt(const u64 index) -> TNode {
    CAPTURE(index);
    const auto ast = astCast<TranslationUnit>(m_ast);
    const auto node = ast.childAt(index);
    return astCast<TNode>(node);
  }

private:
  ASTNode m_ast;
  tl::ExpceptionCollector m_eCollector;
};

TEST_CASE_WITH_FIXTURE("Parser: module, import and type declarations", "[Parser]") {
  REQUIRE_NOTHROW(parse(R"(
module foo;

import std::io;
import std::math;

export type Number = math::Complex | Int | Float;
export type Complex = math::Complex;
  )"));

  auto id = astCast<Identifier>(nodeAt<ModuleDecl>(0).identifier());
  REQUIRE(id.name() == "foo");
  REQUIRE(id.path() == "foo");
  REQUIRE_FALSE(id.imported());
  REQUIRE_FALSE(id.isType());

  id = astCast<Identifier>(nodeAt<ImportDecl>(1).identifier());
  REQUIRE(id.name() == "io");
  REQUIRE(id.path() == "std::io");
  REQUIRE(id.imported());
  REQUIRE_FALSE(id.isType());

  id = astCast<Identifier>(nodeAt<ImportDecl>(2).identifier());
  REQUIRE(id.name() == "math");
  REQUIRE(id.path() == "std::math");
  REQUIRE(id.imported());
  REQUIRE_FALSE(id.isType());

  auto typeDecl = nodeAt<TypeDecl>(3);
  REQUIRE(typeDecl.nChildren() == 2);

  auto typeId = astCast<Identifier>(typeDecl.identifier());
  REQUIRE(typeId.name() == "Number");
  REQUIRE(typeId.path() == "Number");
  REQUIRE(typeId.isType());

  auto typeExpr = astCast<TypeExpr>(typeDecl.typeExpr());
  REQUIRE(typeExpr.nChildren() == 3);

  auto type0 = astCast<Identifier>(typeExpr.type(0));
  REQUIRE(type0.name() == "Complex");
  REQUIRE(type0.path() == "math::Complex");
  REQUIRE(type0.imported());
  REQUIRE(type0.isType());

  auto type1 = astCast<Identifier>(typeExpr.type(1));
  REQUIRE(type1.name() == "Int");
  REQUIRE(type1.path() == "Int");
  REQUIRE_FALSE(type1.imported());
  REQUIRE(type1.isType());

  auto type2 = astCast<Identifier>(typeExpr.type(2));
  REQUIRE(type2.name() == "Float");
  REQUIRE(type2.path() == "Float");
  REQUIRE_FALSE(type2.imported());
  REQUIRE(type2.isType());
}
