#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

using tl::String;
using tl::u64;
using tl::CRef;
using namespace tl::syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(ParserTestFixture, __VA_ARGS__)

#define REQUIRE_VTYPE_OF(variant, Type) \
  REQUIRE(std::derived_from<Type, ASTNodeBase>); \
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

  template<typename TNode>
    requires std::derived_from<TNode, ASTNodeBase>
  auto nodeAt(const u64 index) -> TNode {
    CAPTURE(index);
    const auto ast = std::get<TranslationUnit>(m_ast);
    const auto node = ast.childAt(index);
    REQUIRE_VTYPE_OF(node, TNode);
    return std::get<TNode>(node);
  }

private:
  ASTNode m_ast;
  tl::ExpceptionCollector m_eCollector;
};

TEST_CASE_WITH_FIXTURE("Parser: module and import declarations", "[Parser]") {
  REQUIRE_NOTHROW(parse(R"(
module foo;

import std::io;
import std::math;
  )"));

  nodeAt<ModuleDecl>(0);
  nodeAt<ImportDecl>(1);
  nodeAt<ImportDecl>(2);
}
