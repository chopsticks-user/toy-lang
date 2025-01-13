#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

using tl::String;
using tl::u64;
using tl::sz;
using tl::CRef;
using tl::Opt;
using tl::Vec;
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
    REQUIRE(std::holds_alternative<TranslationUnit>(m_ast));
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

namespace definition {
  TEST_CASE_WITH_FIXTURE("Parser: module, import and type declarations", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo::bar;

import std::io;
import std::math;

export type Number = math::Complex | Int | Float;
type Complex = math::Complex;
      )"));

      auto moduleId = astCast<Identifier>(nodeAt<ModuleDecl>(0).identifier());
      REQUIRE(moduleId.name() == "bar");
      REQUIRE(moduleId.path() == "foo::bar");
      REQUIRE(moduleId.isImported());
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
  }

  TEST_CASE_WITH_FIXTURE("Parser: function definition", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

local fn eval: (x: Float, y: Float) -> (sum: Float, product: Float) {
}

export fn double: (mutable x: Float) -> {
}
      )"));

      auto fn = nodeAt<FunctionDef>(1);
      REQUIRE(fn.storage() == Storage::Local);
      REQUIRE_FALSE(fn.isLambda());

      auto prototype = astCast<FunctionPrototype>(fn.prototype());
      REQUIRE(astCast<Identifier>(prototype.identifier()).name() == "eval");

      // (x: Float, y: Float)
      auto tupleDecl = astCast<TupleDecl>(prototype.params());
      REQUIRE(tupleDecl.size() == 2);

      // x: Float
      auto idDecl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
      REQUIRE_FALSE(idDecl.isMutable());
      auto identifier = astCast<Identifier>(idDecl.identifier());
      REQUIRE(identifier.name() == "x");
      auto typeExpr = astCast<TypeExpr>(idDecl.typeExpr());
      REQUIRE(typeExpr.nTypes() == 1);
      REQUIRE(astCast<Identifier>(typeExpr.type(0)).name() == "Float");

      // y: Float
      idDecl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
      REQUIRE_FALSE(idDecl.isMutable());
      identifier = astCast<Identifier>(idDecl.identifier());
      REQUIRE(identifier.name() == "y");
      typeExpr = astCast<TypeExpr>(idDecl.typeExpr());
      REQUIRE(typeExpr.nTypes() == 1);
      REQUIRE(astCast<Identifier>(typeExpr.type(0)).name() == "Float");

      // (sum: Float, product: Float)
      tupleDecl = astCast<TupleDecl>(prototype.returns());
      REQUIRE(tupleDecl.size() == 2);

      // sum: Float
      idDecl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
      REQUIRE_FALSE(idDecl.isMutable());
      identifier = astCast<Identifier>(idDecl.identifier());
      REQUIRE(identifier.name() == "sum");
      typeExpr = astCast<TypeExpr>(idDecl.typeExpr());
      REQUIRE(typeExpr.nTypes() == 1);
      REQUIRE(astCast<Identifier>(typeExpr.type(0)).name() == "Float");

      // product: Float
      idDecl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
      REQUIRE_FALSE(idDecl.isMutable());
      identifier = astCast<Identifier>(idDecl.identifier());
      REQUIRE(identifier.name() == "product");
      typeExpr = astCast<TypeExpr>(idDecl.typeExpr());
      REQUIRE(typeExpr.nTypes() == 1);
      REQUIRE(astCast<Identifier>(typeExpr.type(0)).name() == "Float");

      fn = nodeAt<FunctionDef>(2);
      REQUIRE(fn.storage() == Storage::Export);
      REQUIRE_FALSE(fn.isLambda());

      prototype = astCast<FunctionPrototype>(fn.prototype());
      REQUIRE(astCast<Identifier>(prototype.identifier()).name() == "double");

      // (mutable x: Float)
      tupleDecl = astCast<TupleDecl>(prototype.params());
      REQUIRE(tupleDecl.size() == 1);

      // mutable x: Float
      idDecl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
      REQUIRE(idDecl.isMutable());
      identifier = astCast<Identifier>(idDecl.identifier());
      REQUIRE(identifier.name() == "x");
      typeExpr = astCast<TypeExpr>(idDecl.typeExpr());
      REQUIRE(typeExpr.nTypes() == 1);
      REQUIRE(astCast<Identifier>(typeExpr.type(0)).name() == "Float");

      // @nothing
      REQUIRE(isEmptyAst(prototype.returns()));
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: concept definition", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

export concept IsComplex {
  fn real: () -> Float | Int;
  fn img: () -> Float | Int;

  fn real: (val: Float | Int) -> Void;
  fn img: (val: Float | Int) -> Void;

  fn +: (other: Complex) -> Complex;
  fn ==: (other: Complex) -> Bool;

  fn conjugate: () -> Complex;
  fn magnitude: () -> Float;
}
      )"));
    }
  }
}

namespace statement {
  TEST_CASE_WITH_FIXTURE("Parser: let statement", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  let x: Int | Float = 10;
  let x = 10.0;
  let mutable x: Int | Float;
  let (mutable x: Int, y: Int | Float) = (10, 5);
  let (x, y) = (10, 5);
  let (_, x) = (10, 5);
  let (_: Int, mutable x) = (10, 5);
  let (_: Int, _) = (10, 5);
  let (Int, Int | Float) = (10, 5);
}
      )"));

      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<LetStmt>(node);
            }
          );

      // let x: Int | Float = 10;
      {
        const auto stmt = statements[0];

        const auto decl = astCast<IdentifierDecl>(stmt.decl());
        REQUIRE_FALSE(decl.isMutable());

        REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

        auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
        REQUIRE(typeExpr.nTypes() == 2);
        REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        REQUIRE(astCast<Identifier>(typeExpr.type(1)).path() == "Float");

        REQUIRE(astCast<IntegerLiteral>(stmt.init()).value() == 10);
      }

      // let x = 10.0;
      {
        const auto stmt = statements[1];

        const auto decl = astCast<IdentifierDecl>(stmt.decl());
        REQUIRE_FALSE(decl.isMutable());

        REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

        REQUIRE(isEmptyAst(decl.typeExpr()));

        REQUIRE(astCast<FloatLiteral>(stmt.init()).value() == 10.0);
      }

      // let mutable x: Int | Float;
      {
        const auto stmt = statements[2];

        const auto decl = astCast<IdentifierDecl>(stmt.decl());
        REQUIRE(decl.isMutable());

        REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

        auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
        REQUIRE(typeExpr.nTypes() == 2);
        REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        REQUIRE(astCast<Identifier>(typeExpr.type(1)).path() == "Float");

        REQUIRE(isEmptyAst(stmt.init()));
      }

      // let (mutable x: Int, y: Int | Float) = (10, 5);
      {
        const auto stmt = statements[3];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

          auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 1);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "y");

          auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 2);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
          REQUIRE(astCast<Identifier>(typeExpr.type(1)).path() == "Float");
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }

      // let (x, y) = (10, 5);
      {
        const auto stmt = statements[4];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "y");

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }

      // let (_, x) = (10, 5);
      {
        const auto stmt = statements[5];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).isAnonymous());

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }

      // let (_: Int, mutable x) = (10, 5);
      {
        const auto stmt = statements[6];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).isAnonymous());

          const auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 1);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).path() == "x");

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }

      // let (_: Int, _) = (10, 5);
      {
        const auto stmt = statements[7];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).isAnonymous());

          const auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 1);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(astCast<Identifier>(decl.identifier()).isAnonymous());

          REQUIRE(isEmptyAst(decl.typeExpr()));
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }

      // let (Int, Int | Float) = (10, 5);
      {
        const auto stmt = statements[8];

        const auto tupleDecl = astCast<TupleDecl>(stmt.decl());
        REQUIRE(tupleDecl.size() == 2);

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(0));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(isEmptyAst(decl.identifier()));

          const auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 1);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
        }

        //
        {
          const auto decl = astCast<IdentifierDecl>(tupleDecl.idDecl(1));
          REQUIRE_FALSE(decl.isMutable());
          REQUIRE(isEmptyAst(decl.identifier()));

          const auto typeExpr = astCast<TypeExpr>(decl.typeExpr());
          REQUIRE(typeExpr.nTypes() == 2);
          REQUIRE(astCast<Identifier>(typeExpr.type(0)).path() == "Int");
          REQUIRE(astCast<Identifier>(typeExpr.type(1)).path() == "Float");
        }

        const auto init = astCast<TupleExpr>(stmt.init());
        REQUIRE(init.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(init.expr(0)).value() == 10);
        REQUIRE(astCast<IntegerLiteral>(init.expr(1)).value() == 5);
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: conditional statement", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  x => {}

  x => func();

  x => y = 7;

  x => return;

  x => y => {}
}
      )"));

      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<ConditionalStmt>(node);
            }
          );

      // x => {}
      {
        const auto stmt = statements[0];

        REQUIRE(astCast<Identifier>(stmt.condition()).path() == "x");
        REQUIRE(matchAstType<BlockStmt>(stmt.body()));
      }

      // x => func();
      {
        const auto stmt = statements[1];

        REQUIRE(astCast<Identifier>(stmt.condition()).path() == "x");

        const auto callExpr = astCast<FunctionCallExpr>(
          astCast<ExprStmt>(stmt.body()).expr()
        );
        REQUIRE(astCast<TupleExpr>(callExpr.args()).size() == 0);
        REQUIRE(astCast<Identifier>(callExpr.callee()).path() == "func");
      }

      // x => y = 7;
      {
        const auto stmt = statements[2];

        REQUIRE(astCast<Identifier>(stmt.condition()).path() == "x");

        const auto assignStmt = astCast<AssignStmt>(stmt.body());
        REQUIRE(assignStmt.op() == "=");
        REQUIRE(astCast<Identifier>(assignStmt.left()).path() == "y");
        REQUIRE(astCast<IntegerLiteral>(assignStmt.right()).value() == 7);
      }

      // x => return;
      {
        const auto stmt = statements[3];
        REQUIRE(astCast<Identifier>(stmt.condition()).path() == "x");
        REQUIRE(isEmptyAst(astCast<ReturnStmt>(stmt.body()).expr()));
      }

      // x => y => {}
      {
        const auto stmt = statements[4];
        REQUIRE(astCast<Identifier>(stmt.condition()).path() == "x");

        const auto innerCondStmt = astCast<ConditionalStmt>(stmt.body());
        REQUIRE(astCast<Identifier>(innerCondStmt.condition()).path() == "y");
        REQUIRE(matchAstType<BlockStmt>(innerCondStmt.body()));
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: assignment statement", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  x = 7;
  x += 7;
  x -= 7;
  x *= 7;
  x /= 7;
  x %= 7;
  x **= 7;
  x &= 7;
  x |= 7;
  x ^= 7;
  x <<= 7;
  x >>= 7;
}
      )"));

      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<AssignStmt>(node);
            }
          );

      REQUIRE(astCast<Identifier>(statements[0].left()).path() == "x");
      REQUIRE(statements[0].op() == "=");
      REQUIRE(astCast<IntegerLiteral>(statements[0].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[1].left()).path() == "x");
      REQUIRE(statements[1].op() == "+=");
      REQUIRE(astCast<IntegerLiteral>(statements[1].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[2].left()).path() == "x");
      REQUIRE(statements[2].op() == "-=");
      REQUIRE(astCast<IntegerLiteral>(statements[2].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[3].left()).path() == "x");
      REQUIRE(statements[3].op() == "*=");
      REQUIRE(astCast<IntegerLiteral>(statements[3].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[4].left()).path() == "x");
      REQUIRE(statements[4].op() == "/=");
      REQUIRE(astCast<IntegerLiteral>(statements[4].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[5].left()).path() == "x");
      REQUIRE(statements[5].op() == "%=");
      REQUIRE(astCast<IntegerLiteral>(statements[5].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[6].left()).path() == "x");
      REQUIRE(statements[6].op() == "**=");
      REQUIRE(astCast<IntegerLiteral>(statements[6].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[7].left()).path() == "x");
      REQUIRE(statements[7].op() == "&=");
      REQUIRE(astCast<IntegerLiteral>(statements[7].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[8].left()).path() == "x");
      REQUIRE(statements[8].op() == "|=");
      REQUIRE(astCast<IntegerLiteral>(statements[8].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[9].left()).path() == "x");
      REQUIRE(statements[9].op() == "^=");
      REQUIRE(astCast<IntegerLiteral>(statements[9].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[10].left()).path() == "x");
      REQUIRE(statements[10].op() == "<<=");
      REQUIRE(astCast<IntegerLiteral>(statements[10].right()).value() == 7);

      REQUIRE(astCast<Identifier>(statements[11].left()).path() == "x");
      REQUIRE(statements[11].op() == ">>=");
      REQUIRE(astCast<IntegerLiteral>(statements[11].right()).value() == 7);
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: expression statement", "[Parser]") {
    SECTION("Simple") {
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: return statement", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn foo: () -> {
  return x;
  return (x, y);
}
      )"));

      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<ReturnStmt>(node);
            }
          );

      //
      {
        // REQUIRE(astCast<Identifier>())
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: match statement", "[Parser]") {
    SECTION("Zero arguments") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  match {
    x && y => {},
    _ => {}
  }
}
      )"));

      const auto matchStmt = astCast<MatchStmt>(
        astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).stmt(0)
      );

      REQUIRE(matchStmt.nExprs() == 0);
      REQUIRE(matchStmt.nCases() == 2);
      REQUIRE(matchAstType<BlockStmt>(matchStmt.defaultCase()));

      const auto matchingCase = astCast<MatchStmtCase>(matchStmt.caseAt(0));
      REQUIRE(matchAstType<BlockStmt>(matchingCase.body()));
      REQUIRE(isEmptyAst(matchingCase.value()));

      const auto condition = astCast<BinaryExpr>(matchingCase.condition());
      REQUIRE(condition.op() == "&&");
      REQUIRE(astCast<Identifier>(condition.left()).path() == "x");
      REQUIRE(astCast<Identifier>(condition.right()).path() == "y");
    }

    SECTION("One argument") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  match x {
    10 when x > 5 && y > 5 => {},
    _ when x > 5 && y > 5 => {},
    _ => {}
  }
}
      )"));

      const auto matchStmt = astCast<MatchStmt>(
        astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).stmt(0)
      );

      REQUIRE(matchStmt.nExprs() == 1);
      REQUIRE(matchStmt.nCases() == 3);
      REQUIRE(matchAstType<BlockStmt>(matchStmt.defaultCase()));

      //
      {
        const auto matchingCase = astCast<MatchStmtCase>(matchStmt.caseAt(0));
        REQUIRE(matchAstType<BlockStmt>(matchingCase.body()));

        REQUIRE(astCast<IntegerLiteral>(matchingCase.value()).value() == 10);

        const auto condition = astCast<BinaryExpr>(matchingCase.condition());
        REQUIRE(condition.op() == "&&");
        REQUIRE(astCast<BinaryExpr>(condition.left()).op() == ">");
        REQUIRE(astCast<BinaryExpr>(condition.right()).op() == ">");
      }

      //
      {
        const auto matchingCase = astCast<MatchStmtCase>(matchStmt.caseAt(1));
        REQUIRE(matchAstType<BlockStmt>(matchingCase.body()));

        REQUIRE(astCast<Identifier>(matchingCase.value()).isAnonymous());

        const auto condition = astCast<BinaryExpr>(matchingCase.condition());
        REQUIRE(condition.op() == "&&");
        REQUIRE(astCast<BinaryExpr>(condition.left()).op() == ">");
        REQUIRE(astCast<BinaryExpr>(condition.right()).op() == ">");
      }
    }

    SECTION("Two arguments") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  match (x, y) {
    (5, "string") when z == 0 => {},
    (_, _) when z < 0 && x == 10 => {},
    _ => {}
  }
}
      )"));

      const auto matchStmt = astCast<MatchStmt>(
        astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).stmt(0)
      );

      REQUIRE(matchStmt.nExprs() == 2);
      REQUIRE(matchStmt.nCases() == 3);
      REQUIRE(matchAstType<BlockStmt>(matchStmt.defaultCase()));

      //
      {
        const auto matchingCase = astCast<MatchStmtCase>(matchStmt.caseAt(0));
        REQUIRE(matchAstType<BlockStmt>(matchingCase.body()));

        const auto value = astCast<TupleExpr>(matchingCase.value());
        REQUIRE(value.size() == 2);
        REQUIRE(astCast<IntegerLiteral>(value.expr(0)).value() == 5);
        REQUIRE(astCast<StringLiteral>(value.expr(1)).value() == "\"string\"");

        const auto condition = astCast<BinaryExpr>(matchingCase.condition());
        REQUIRE(condition.op() == "==");
        REQUIRE(astCast<Identifier>(condition.left()).path() == "z");
        REQUIRE(astCast<IntegerLiteral>(condition.right()).value() == 0);
      }

      //
      {
        const auto matchingCase = astCast<MatchStmtCase>(matchStmt.caseAt(1));
        REQUIRE(matchAstType<BlockStmt>(matchingCase.body()));

        const auto value = astCast<TupleExpr>(matchingCase.value());
        REQUIRE(value.size() == 2);
        REQUIRE(astCast<Identifier>(value.expr(0)).isAnonymous());
        REQUIRE(astCast<Identifier>(value.expr(1)).isAnonymous());

        const auto condition = astCast<BinaryExpr>(matchingCase.condition());
        REQUIRE(condition.op() == "&&");
        REQUIRE(astCast<BinaryExpr>(condition.left()).op() == "<");
        REQUIRE(astCast<BinaryExpr>(condition.right()).op() == "==");
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: for statement", "[Parser]") {
    SECTION("Simple") {
    }
  }
}

namespace expression {
}
