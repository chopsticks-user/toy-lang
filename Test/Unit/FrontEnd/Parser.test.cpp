#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Parser.hpp"

using tl::String;
using tl::sz;
using tl::CRef;
using tl::Opt;
using tl::Vec;
using namespace tl::syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
  TEST_CASE_METHOD(ParserTestFixture, __VA_ARGS__)

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
  auto nodeAt(const sz index) -> TNode {
    CAPTURE(index);
    const auto ast = astCast<TranslationUnit>(m_ast);
    const auto node = ast.childAt(index);
    return astCast<TNode>(node);
  }

  template<std::derived_from<ASTNodeBase> TNode>
  auto statementsInFnBodyAt(const sz index) -> Vec<TNode> {
    auto view = astCast<BlockStmt>(nodeAt<FunctionDef>(index).body()).view() |
                rv::transform(
                  [](CRef<ASTNode> node) {
                    return astCast<TNode>(node);
                  }
                );
    return {view.begin(), view.end()};
  }

private:
  ASTNode m_ast;
  tl::ExpceptionCollector m_eCollector;
};

namespace expression {
  TEST_CASE_WITH_FIXTURE("Parser: primary expression", "[Parser]") {
    SECTION("Number") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  7;
  0;
  3.14159;
  0.0;
  .25;
  .0;
}
      )"));

      const auto statements = statementsInFnBodyAt<ExprStmt>(1);
      REQUIRE(astCast<IntegerLiteral>(statements[0].expr()).value() == 7);
      REQUIRE(astCast<IntegerLiteral>(statements[1].expr()).value() == 0);
      REQUIRE(astCast<FloatLiteral>(statements[2].expr()).value() == 3.14159);
      REQUIRE(astCast<FloatLiteral>(statements[3].expr()).value() == 0.0);
      REQUIRE(astCast<FloatLiteral>(statements[4].expr()).value() == 0.25);
      REQUIRE(astCast<FloatLiteral>(statements[5].expr()).value() == 0.0);
    }

    SECTION("Boolean") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  false;
  true;
}
      )"));

      const auto statements = statementsInFnBodyAt<ExprStmt>(1);
      REQUIRE(astCast<BooleanLiteral>(statements[0].expr()).value() == false);
      REQUIRE(astCast<BooleanLiteral>(statements[1].expr()).value() == true);
    }

    SECTION("String") {
      // todo: Lexer::lexStringLiteral
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  "string";
  "other string \n";
}
      )"));

      const auto statements = statementsInFnBodyAt<ExprStmt>(1);
      REQUIRE(astCast<StringLiteral>(statements[0].expr()).value() == "\"string\"");
      REQUIRE(astCast<StringLiteral>(statements[1].expr()).value() == "\"other string \\n\"");
    }

    SECTION("Identifier") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  variable;
  foo::bar::x;
  Type;
  bar::Type;
  _;
}
      )"));

      const auto statements = statementsInFnBodyAt<ExprStmt>(1);

      //
      {
        const auto id = astCast<Identifier>(statements[0].expr());
        REQUIRE(id.name() == "variable");
        REQUIRE(id.path() == "variable");
        REQUIRE_FALSE(id.isAnonymous());
        REQUIRE_FALSE(id.isImported());
        REQUIRE_FALSE(id.isType());
        REQUIRE_FALSE(id.isOverloadedOp());
      }

      //
      {
        const auto id = astCast<Identifier>(statements[1].expr());
        REQUIRE(id.name() == "x");
        REQUIRE(id.path() == "foo::bar::x");
        REQUIRE_FALSE(id.isAnonymous());
        REQUIRE(id.isImported());
        REQUIRE_FALSE(id.isType());
        REQUIRE_FALSE(id.isOverloadedOp());
      }

      //
      {
        const auto id = astCast<Identifier>(statements[2].expr());
        REQUIRE(id.name() == "Type");
        REQUIRE(id.path() == "Type");
        REQUIRE_FALSE(id.isAnonymous());
        REQUIRE_FALSE(id.isImported());
        REQUIRE(id.isType());
        REQUIRE_FALSE(id.isOverloadedOp());
      }

      //
      {
        const auto id = astCast<Identifier>(statements[3].expr());
        REQUIRE(id.name() == "Type");
        REQUIRE(id.path() == "bar::Type");
        REQUIRE_FALSE(id.isAnonymous());
        REQUIRE(id.isImported());
        REQUIRE(id.isType());
        REQUIRE_FALSE(id.isOverloadedOp());
      }

      //
      {
        const auto id = astCast<Identifier>(statements[4].expr());
        REQUIRE(id.name() == "");
        REQUIRE(id.path() == "");
        REQUIRE(id.isAnonymous());
        REQUIRE_FALSE(id.isImported());
        REQUIRE_FALSE(id.isType());
        REQUIRE_FALSE(id.isOverloadedOp());
      }
    }

    SECTION("Tuple") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  ();
  (x);
  (x, 7);
  (x, _);
  (x, 7, "string");
  (_, _, _);
}
      )"));

      const auto expressions =
          statementsInFnBodyAt<ExprStmt>(1) |
          rv::transform(
            [](CRef<ExprStmt> stmt) {
              return astCast<TupleExpr>(stmt.expr());
            }
          );

      //
      {
        REQUIRE(expressions[0].size() == 0);
      }

      //
      {
        REQUIRE(expressions[1].size() == 1);
        REQUIRE(astCast<Identifier>(expressions[1].expr(0)).path() == "x");
      }

      //
      {
        REQUIRE(expressions[2].size() == 2);
        REQUIRE(astCast<Identifier>(expressions[2].expr(0)).path() == "x");
        REQUIRE(astCast<IntegerLiteral>(expressions[2].expr(1)).value() == 7);
      }

      //
      {
        REQUIRE(expressions[3].size() == 2);
        REQUIRE(astCast<Identifier>(expressions[3].expr(0)).path() == "x");
        REQUIRE(astCast<Identifier>(expressions[3].expr(1)).isAnonymous());
      }

      //
      {
        REQUIRE(expressions[4].size() == 3);
        REQUIRE(astCast<Identifier>(expressions[4].expr(0)).path() == "x");
        REQUIRE(astCast<IntegerLiteral>(expressions[4].expr(1)).value() == 7);
        REQUIRE(astCast<StringLiteral>(expressions[4].expr(2)).value() == "\"string\"");
      }

      //
      {
        REQUIRE(expressions[5].size() == 3);
        REQUIRE(astCast<Identifier>(expressions[5].expr(0)).isAnonymous());
        REQUIRE(astCast<Identifier>(expressions[5].expr(1)).isAnonymous());
        REQUIRE(astCast<Identifier>(expressions[5].expr(2)).isAnonymous());
      }
    }

    SECTION("Array") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  [];
  [x];
  [x, 7];
  [x, []];
  [x, y, ...arr];
  [x, y, ...[], 0 .. 7];
}
      )"));

      const auto expressions =
          statementsInFnBodyAt<ExprStmt>(1) |
          rv::transform(
            [](CRef<ExprStmt> stmt) {
              return astCast<ArrayExpr>(stmt.expr());
            }
          );

      //
      {
        REQUIRE(expressions[0].size() == 0);
      }

      //
      {
        REQUIRE(expressions[1].size() == 1);
        REQUIRE(astCast<Identifier>(expressions[1].element(0)).path() == "x");
      }

      //
      {
        REQUIRE(expressions[2].size() == 2);
        REQUIRE(astCast<Identifier>(expressions[2].element(0)).path() == "x");
        REQUIRE(astCast<IntegerLiteral>(expressions[2].element(1)).value() == 7);
      }

      //
      {
        REQUIRE(expressions[3].size() == 2);
        REQUIRE(astCast<Identifier>(expressions[3].element(0)).path() == "x");
        REQUIRE(astCast<ArrayExpr>(expressions[3].element(1)).size() == 0);
      }

      //
      {
        REQUIRE(expressions[4].size() == 3);
        REQUIRE(astCast<Identifier>(expressions[4].element(0)).path() == "x");
        REQUIRE(astCast<Identifier>(expressions[4].element(1)).path() == "y");

        const auto rng = astCast<UnaryExpr>(expressions[4].element(2));
        REQUIRE(rng.op() == "...");
        REQUIRE(astCast<Identifier>(rng.operand()).path() == "arr");
      }

      //
      {
        REQUIRE(expressions[5].size() == 4);
        REQUIRE(astCast<Identifier>(expressions[5].element(0)).path() == "x");
        REQUIRE(astCast<Identifier>(expressions[5].element(1)).path() == "y");

        const auto rng = astCast<UnaryExpr>(expressions[5].element(2));
        REQUIRE(rng.op() == "...");
        REQUIRE(astCast<ArrayExpr>(rng.operand()).size() == 0);

        const auto seq = astCast<TernaryExpr>(expressions[5].element(3));
        REQUIRE(seq.firstOp() == "..");
        REQUIRE(seq.secondOp() == "@");
        REQUIRE(astCast<IntegerLiteral>(seq.first()).value() == 0);
        REQUIRE(astCast<IntegerLiteral>(seq.second()).value() == 7);
        REQUIRE(astCast<IntegerLiteral>(seq.third()).value() == 1);
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: postfix expression", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  func();
  x |> func(y);
  arr[0];
  obj.field;
}
      )"));

      const auto statements = statementsInFnBodyAt<ExprStmt>(1);

      //
      {
        const auto expr = astCast<FunctionCallExpr>(statements[0].expr());
        REQUIRE(astCast<Identifier>(expr.callee()).path() == "func");
        REQUIRE(astCast<TupleExpr>(expr.args()).size() == 0);
      }

      //
      {
        const auto expr = astCast<FunctionCallExpr>(statements[1].expr());
        REQUIRE(astCast<Identifier>(expr.callee()).path() == "func");

        const auto args = astCast<TupleExpr>(expr.args());
        REQUIRE(args.size() == 2);
        REQUIRE(astCast<Identifier>(args.expr(0)).path() == "x");
        REQUIRE(astCast<Identifier>(args.expr(1)).path() == "y");
      }

      //
      {
        const auto expr = astCast<SubScriptingExpr>(statements[2].expr());
        REQUIRE(astCast<Identifier>(expr.collection()).path() == "arr");
        REQUIRE(astCast<IntegerLiteral>(expr.subscript()).value() == 0);
      }

      //
      {
        const auto expr = astCast<AccessExpr>(statements[3].expr());
        REQUIRE(astCast<Identifier>(expr.object()).path() == "obj");
        REQUIRE(astCast<Identifier>(expr.field()).path() == "field");
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: prefix expression", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  !true;
  ~x;
  +7;
  -3.14;
  #[];
  ...arr;
}
      )"));

      const auto expressions =
          statementsInFnBodyAt<ExprStmt>(1) | rv::transform(
            [](CRef<ExprStmt> stmt) { return astCast<UnaryExpr>(stmt.expr()); }
          );

      //
      {
        REQUIRE(expressions[0].op() == "!");
        REQUIRE(astCast<BooleanLiteral>(expressions[0].operand()).value() == true);
      }

      //
      {
        REQUIRE(expressions[1].op() == "~");
        REQUIRE(astCast<Identifier>(expressions[1].operand()).path() == "x");
      }

      //
      {
        REQUIRE(expressions[2].op() == "+");
        REQUIRE(astCast<IntegerLiteral>(expressions[2].operand()).value() == 7);
      }

      //
      {
        REQUIRE(expressions[3].op() == "-");
        REQUIRE(astCast<FloatLiteral>(expressions[3].operand()).value() == 3.14);
      }

      //
      {
        REQUIRE(expressions[4].op() == "#");
        REQUIRE(astCast<ArrayExpr>(expressions[4].operand()).size() == 0);
      }

      //
      {
        REQUIRE(expressions[5].op() == "...");
        REQUIRE(astCast<Identifier>(expressions[5].operand()).path() == "arr");
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: binary expression", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  x ** y;
  x * y;
  x / y;
  x % y;
  x + y;
  x - y;
  x << y;
  x >> y;
  x < y;
  x > y;
  x <= y;
  x >= y;
  x == y;
  x != y;
  x & y;
  x ^ y;
  x | y;
  x && y;
  x || y;
  x ?? y;
}
      )"));

      const auto operators =
          statementsInFnBodyAt<ExprStmt>(1) | rv::transform(
            [](CRef<ExprStmt> stmt) { return astCast<BinaryExpr>(stmt.expr()).op(); }
          );

      REQUIRE(operators[0] == "**");
      REQUIRE(operators[1] == "*");
      REQUIRE(operators[2] == "/");
      REQUIRE(operators[3] == "%");
      REQUIRE(operators[4] == "+");
      REQUIRE(operators[5] == "-");
      REQUIRE(operators[6] == "<<");
      REQUIRE(operators[7] == ">>");
      REQUIRE(operators[8] == "<");
      REQUIRE(operators[9] == ">");
      REQUIRE(operators[10] == "<=");
      REQUIRE(operators[11] == ">=");
      REQUIRE(operators[12] == "==");
      REQUIRE(operators[13] == "!=");
      REQUIRE(operators[14] == "&");
      REQUIRE(operators[15] == "^");
      REQUIRE(operators[16] == "|");
      REQUIRE(operators[17] == "&&");
      REQUIRE(operators[18] == "||");
      REQUIRE(operators[19] == "??");
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: ternary expression", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(module foo;
fn main: () -> {
  x ? y : z;
  x .. y @ z;
}
      )"));
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: precedence", "[Parser]") {
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
    SECTION("Function call") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  func();
  x |> func(y);
}
      )"));

      const auto expressions =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<FunctionCallExpr>(astCast<ExprStmt>(node).expr());
            }
          );

      //
      {
        REQUIRE(astCast<Identifier>(expressions[0].callee()).path() == "func");

        const auto args = astCast<TupleExpr>(expressions[0].args());
        REQUIRE(args.size() == 0);
      }

      //
      {
        REQUIRE(astCast<Identifier>(expressions[1].callee()).path() == "func");

        const auto args = astCast<TupleExpr>(expressions[1].args());
        REQUIRE(args.size() == 2);
        REQUIRE(astCast<Identifier>(args.expr(0)).path() == "x");
        REQUIRE(astCast<Identifier>(args.expr(1)).path() == "y");
      }
    }
  }

  TEST_CASE_WITH_FIXTURE("Parser: return statement", "[Parser]") {
    SECTION("Simple") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn foo: () -> {
  return x;
  return (x, y, z);
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
        REQUIRE(astCast<Identifier>(statements[0].expr()).path() == "x");
      }

      //
      {
        const auto tuple = astCast<TupleExpr>(statements[1].expr());
        REQUIRE(tuple.size() == 3);
        REQUIRE(astCast<Identifier>(tuple.expr(0)).path() == "x");
        REQUIRE(astCast<Identifier>(tuple.expr(1)).path() == "y");
        REQUIRE(astCast<Identifier>(tuple.expr(2)).path() == "z");
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
    SECTION("For until") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  for expr {}

  for true {}

  for x && y {}
}
      )"));


      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<ForStmt>(node);
            }
          );

      //
      {
        REQUIRE_FALSE(statements[0].isForRange());
        REQUIRE(astCast<Identifier>(statements[0].condition()).path() == "expr");
        REQUIRE(matchAstType<BlockStmt>(statements[0].body()));
      }

      //
      {
        REQUIRE_FALSE(statements[1].isForRange());
        REQUIRE(astCast<BooleanLiteral>(statements[1].condition()).value() == true);
        REQUIRE(matchAstType<BlockStmt>(statements[2].body()));
      }

      //
      {
        REQUIRE_FALSE(statements[2].isForRange());
        const auto cond = astCast<BinaryExpr>(statements[2].condition());
        REQUIRE(cond.op() == "&&");
        REQUIRE(astCast<Identifier>(cond.left()).path() == "x");
        REQUIRE(astCast<Identifier>(cond.right()).path() == "y");
        REQUIRE(matchAstType<BlockStmt>(statements[2].body()));
      }
    }

    SECTION("For range") {
      REQUIRE_NOTHROW(parse(R"(
module foo;

fn main: () -> {
  for (index: Int, value) in ...[0 .. 11 @ 2] {}

  for (_, value) in 0 .. 11 @ 2 {}

  for (index: Int, value) in ...coll {}

  for (_, (r: Int, l: Float)) in rng {}
}
      )"));

      const auto statements =
          astCast<BlockStmt>(nodeAt<FunctionDef>(1).body()).view() | rv::transform(
            [](CRef<ASTNode> node) {
              return astCast<ForStmt>(node);
            }
          );

      //
      {
        REQUIRE(statements[0].isForRange());
        const auto fragment = astCast<ForRangeFragment>(statements[0].condition());
        REQUIRE(astCast<TupleDecl>(fragment.iterator()).size() == 2);

        const auto iterable = astCast<UnaryExpr>(fragment.iterable());
        REQUIRE(iterable.op() == "...");
        REQUIRE(matchAstType<ArrayExpr>(iterable.operand()));
        REQUIRE(astCast<ArrayExpr>(iterable.operand()).size() == 1);
        REQUIRE(matchAstType<TernaryExpr>(astCast<ArrayExpr>(iterable.operand()).element(0)));
      }

      //
      {
        REQUIRE(statements[1].isForRange());
        const auto fragment = astCast<ForRangeFragment>(statements[1].condition());
        REQUIRE(astCast<TupleDecl>(fragment.iterator()).size() == 2);

        const auto iterable = astCast<TernaryExpr>(fragment.iterable());
        REQUIRE(iterable.firstOp() == "..");
        REQUIRE(iterable.secondOp() == "@");
        REQUIRE(astCast<IntegerLiteral>(iterable.first()).value() == 0);
        REQUIRE(astCast<IntegerLiteral>(iterable.second()).value() == 11);
        REQUIRE(astCast<IntegerLiteral>(iterable.third()).value() == 2);
      }

      //
      {
        REQUIRE(statements[2].isForRange());
        const auto fragment = astCast<ForRangeFragment>(statements[2].condition());
        REQUIRE(astCast<TupleDecl>(fragment.iterator()).size() == 2);

        const auto iterable = astCast<UnaryExpr>(fragment.iterable());
        REQUIRE(iterable.op() == "...");
        REQUIRE(astCast<Identifier>(iterable.operand()).path() == "coll");
      }

      //
      {
        REQUIRE(statements[3].isForRange());
        const auto fragment = astCast<ForRangeFragment>(statements[3].condition());
        REQUIRE(astCast<TupleDecl>(fragment.iterator()).size() == 2);

        REQUIRE(astCast<Identifier>(fragment.iterable()).path() == "rng");
      }
    }
  }
}

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
