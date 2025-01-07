#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

using tl::String;
using tl::u64;
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
type Complex = math::Complex;
  )"));

  auto moduleId = astCast<Identifier>(nodeAt<ModuleDecl>(0).identifier());
  REQUIRE(moduleId.name() == "foo");
  REQUIRE(moduleId.path() == "foo");
  REQUIRE_FALSE(moduleId.isImported());
  REQUIRE_FALSE(moduleId.isType());

  auto importId0 = astCast<Identifier>(nodeAt<ImportDecl>(1).identifier());
  REQUIRE(importId0.name() == "io");
  REQUIRE(importId0.path() == "std::io");
  REQUIRE(importId0.isImported());
  REQUIRE_FALSE(importId0.isType());

  auto importId1 = astCast<Identifier>(nodeAt<ImportDecl>(2).identifier());
  REQUIRE(importId1.name() == "math");
  REQUIRE(importId1.path() == "std::math");
  REQUIRE(importId1.isImported());
  REQUIRE_FALSE(importId1.isType());

  auto typeDecl0 = nodeAt<TypeDecl>(3);
  REQUIRE(typeDecl0.nChildren() == 2);

  auto typeId0 = astCast<Identifier>(typeDecl0.identifier());
  REQUIRE(typeId0.name() == "Number");
  REQUIRE(typeId0.path() == "Number");
  REQUIRE(typeId0.isType());

  auto typeExpr0 = astCast<TypeExpr>(typeDecl0.typeExpr());
  REQUIRE(typeExpr0.nChildren() == 3);

  auto type00 = astCast<Identifier>(typeExpr0.type(0));
  REQUIRE(type00.name() == "Complex");
  REQUIRE(type00.path() == "math::Complex");
  REQUIRE(type00.isImported());
  REQUIRE(type00.isType());

  auto type01 = astCast<Identifier>(typeExpr0.type(1));
  REQUIRE(type01.name() == "Int");
  REQUIRE(type01.path() == "Int");
  REQUIRE_FALSE(type01.isImported());
  REQUIRE(type01.isType());

  auto type02 = astCast<Identifier>(typeExpr0.type(2));
  REQUIRE(type02.name() == "Float");
  REQUIRE(type02.path() == "Float");
  REQUIRE_FALSE(type02.isImported());
  REQUIRE(type02.isType());

  auto typeDecl1 = nodeAt<TypeDecl>(4);
  REQUIRE(typeDecl1.nChildren() == 2);

  auto typeId1 = astCast<Identifier>(typeDecl1.identifier());
  REQUIRE(typeId1.name() == "Complex");
  REQUIRE(typeId1.path() == "Complex");
  REQUIRE(typeId1.isType());

  auto typeExpr1 = astCast<TypeExpr>(typeDecl1.typeExpr());
  REQUIRE(typeExpr1.nChildren() == 1);

  auto type10 = astCast<Identifier>(typeExpr1.type(0));
  REQUIRE(type10.name() == "Complex");
  REQUIRE(type10.path() == "math::Complex");
  REQUIRE(type10.isImported());
  REQUIRE(type10.isType());
}
