#include "FrontEnd/CombinatoryParser.hpp"

#include <catch2/catch_test_macros.hpp>

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

  template<typename>
  struct LambdaTag {
  };

  template<IsASTNode T = ASTNode>
  static auto parse(String source, CRef<CombUnitParser> parser) -> std::tuple<T, Strings, ParsingDriver> {
    std::istringstream iss;
    iss.str(std::move(source));
    auto driver = ParsingDriver{
      filepath, tl::apply<Lexer>(std::move(iss))
    };

    auto [nodes, errors] = parser(driver);
    REQUIRE_FALSE(nodes.empty());
    if constexpr (std::same_as<T, ASTNode>) {
      return {std::move(nodes), std::move(errors), std::move(driver)};
    } else {
      return {astCast<T>(nodes.front()), std::move(errors), std::move(driver)};
    }
  }

  template<IsASTNode T = ASTNode>
  static auto parseNoThrow(String source, CRef<CombUnitParser> parser) -> T {
    auto [node, errors, driver] = parse<T>(std::move(source), parser);
    REQUIRE(driver.done());
    REQUIRE(errors.empty());
    return node;
  }
};

TEST_CASE_WITH_FIXTURE("Parser: literals", "[Parser]") {
  SECTION("Integer") {
    auto i = parseNoThrow<IntegerLiteral>("0 ", CombParser::integerExpr());
    REQUIRE(i.value() == 0);

    i = parseNoThrow<IntegerLiteral>("00000 ", CombParser::integerExpr());
    REQUIRE(i.value() == 0);

    i = parseNoThrow<IntegerLiteral>("177 ", CombParser::integerExpr());
    REQUIRE(i.value() == 177);

    i = parseNoThrow<IntegerLiteral>("1073741824 ", CombParser::integerExpr());
    REQUIRE(i.value() == 1073741824);
  }

  SECTION("Float") {
    auto f = parseNoThrow<FloatLiteral>("0.0 ", CombParser::floatExpr());
    REQUIRE(f.value() == 0.0);

    f = parseNoThrow<FloatLiteral>("1.0000 ", CombParser::floatExpr());
    REQUIRE(f.value() == 1.0);

    f = parseNoThrow<FloatLiteral>(".000000 ", CombParser::floatExpr());
    REQUIRE(f.value() == 0.0);

    f = parseNoThrow<FloatLiteral>("221212.177 ", CombParser::floatExpr());
    REQUIRE(f.value() == 221212.177);

    f = parseNoThrow<FloatLiteral>(".177 ", CombParser::floatExpr());
    REQUIRE(f.value() == 0.177);

    f = parseNoThrow<FloatLiteral>("589217.1073741824 ", CombParser::floatExpr());
    REQUIRE(f.value() == 589217.1073741824);

    f = parseNoThrow<FloatLiteral>(".1073741824 ", CombParser::floatExpr());
    REQUIRE(f.value() == 0.1073741824);
  }

  SECTION("Boolean") {
    auto b = parseNoThrow<BooleanLiteral>("true ", CombParser::booleanExpr());
    REQUIRE(b.value() == true);

    b = parseNoThrow<BooleanLiteral>("false ", CombParser::booleanExpr());
    REQUIRE(b.value() == false);
  }

  SECTION("String") {
    auto s = parseNoThrow<StringLiteral>(R"("" )", CombParser::stringExpr());
    REQUIRE(s.value() == "\"\"");

    s = parseNoThrow<StringLiteral>(R"("hello world!" )", CombParser::stringExpr());
    REQUIRE(s.value() == "\"hello world!\"");

    s = parseNoThrow<StringLiteral>(R"("string\n\r\t" )", CombParser::stringExpr());
    REQUIRE(s.value() == "\"string\\n\\r\\t\"");
  }
}

TEST_CASE_WITH_FIXTURE("Parser: variable identifier", "[Parser]") {
  struct Expected {
    const String name;
    const String path;
    const bool isAnonymous;
    const bool isImported;
  };

  static constexpr auto assertVarId = [](String source, Expected &&expected) {
    const auto id = parseNoThrow<VarId>(std::move(source), CombParser::varIdExpr());
    REQUIRE(id.name() == expected.name);
    REQUIRE(id.path() == expected.path);
    REQUIRE(id.isAnonymous() == expected.isAnonymous);
    REQUIRE(id.isImported() == expected.isImported);
  };

  assertVarId(
    "_ ",
    {.name = "", .path = "", .isAnonymous = true, .isImported = false}
  );

  assertVarId(
    "x ",
    {.name = "x", .path = "x", .isAnonymous = false, .isImported = false}
  );

  assertVarId(
    "foo::bar::var ",
    {.name = "var", .path = "foo::bar::var", .isAnonymous = false, .isImported = true}
  );
}

TEST_CASE_WITH_FIXTURE("Parser: type identifier", "[Parser]") {
  struct Expected {
    const String name = "";
    const String path = "";
    const bool isImported = false;
  };

  static constexpr auto assertTypeId = [](String source, Expected &&expected) {
    const auto id = parseNoThrow<TypeId>(std::move(source), CombParser::typeIdExpr());
    REQUIRE(id.name() == expected.name);
    REQUIRE(id.path() == expected.path);
    REQUIRE(id.isImported() == expected.isImported);
  };

  assertTypeId(
    "Type ",
    {.name = "Type", .path = "Type", .isImported = false}
  );

  assertTypeId(
    "foo::bar::Int  ",
    {.name = "Int", .path = "foo::bar::Int", .isImported = true}
  );
}

TEST_CASE_WITH_FIXTURE("Parser: identifier declaration", "[Parser]") {
  struct Expected {
    const bool isMutable = false;
    const Opt<String> varPath = {};
    const Opt<String> typePath = {};
  };

  static constexpr auto assertIdDecl = [](String source, Expected &&expected) {
    const auto decl = parseNoThrow<IdentifierDecl>(std::move(source), CombParser::identifierDecl());

    REQUIRE(decl.isMutable() == expected.isMutable);

    if (expected.varPath.has_value()) {
      REQUIRE(astCast<VarId>(decl.identifier()).path() == expected.varPath.value());
    } else {
      REQUIRE(isEmptyAst(decl.identifier()));
    }

    if (expected.typePath.has_value()) {
      REQUIRE(astCast<TypeId>(decl.type()).path() == expected.typePath.value());
    } else {
      REQUIRE(isEmptyAst(decl.type()));
    }
  };

  assertIdDecl(
    "foo: Int ",
    {.isMutable = false, .varPath = "foo", .typePath = "Int"}
  );

  assertIdDecl(
    "mut bar: Type ",
    {.isMutable = true, .varPath = "bar", .typePath = "Type"}
  );

  assertIdDecl(
    "_: Float ",
    {.isMutable = false, .varPath = "", .typePath = "Float"}
  );

  assertIdDecl(
    "foo ",
    {.isMutable = false, .varPath = "foo"}
  );

  assertIdDecl(
    "mut foo ",
    {.isMutable = true, .varPath = "foo"}
  );

  assertIdDecl(
    "_ ",
    {.isMutable = false, .varPath = ""}
  );

  assertIdDecl(
    "Int ",
    {.isMutable = false, .typePath = "Int"}
  );

  assertIdDecl(
    "foo::bar::Type ",
    {.isMutable = false, .typePath = "foo::bar::Type"}
  );
}

TEST_CASE_WITH_FIXTURE("Parser: tuple declaration", "[Parser]") {
  struct Expected {
    const bool isMutable = false;
    const Opt<String> varPath = {};
    const Opt<String> typePath = {};
  };

  static constexpr auto assertTupleDecl = [](String source, CRef<Vec<Expected> > expected) {
    const auto tupleDecl = parseNoThrow<TupleDecl>(std::move(source), CombParser::tupleDecl());
    REQUIRE(tupleDecl.size() == expected.size());

    auto it = tupleDecl.children().begin();
    for (auto &[isMutable, varPath, typePath]: expected) {
      const auto decl = astCast<IdentifierDecl>(*it++);

      REQUIRE(decl.isMutable() == isMutable);

      if (varPath.has_value()) {
        REQUIRE(astCast<VarId>(decl.identifier()).path() == varPath.value());
      } else {
        REQUIRE(isEmptyAst(decl.identifier()));
      }

      if (typePath.has_value()) {
        REQUIRE(astCast<TypeId>(decl.type()).path() == typePath.value());
      } else {
        REQUIRE(isEmptyAst(decl.type()));
      }
    }
  };

  assertTupleDecl("() ", {});

  assertTupleDecl("(mut x) ", {{.isMutable = true, .varPath = "x"}});

  assertTupleDecl(
    "(var: foo::Var) ",
    {
      {.isMutable = false, .varPath = "var", .typePath = "foo::Var"}
    }
  );

  assertTupleDecl("(Int) ", {{.isMutable = false, .typePath = "Int"}});

  assertTupleDecl(
    "(foo::Var, Int, Float) ",
    {
      {.isMutable = false, .typePath = "foo::Var"},
      {.isMutable = false, .typePath = "Int"},
      {.isMutable = false, .typePath = "Float"}
    }
  );

  assertTupleDecl(
    "(mut var1: Var1, _, var2: foo::bar::Var2, _: Int, Float) ",
    {
      {.isMutable = true, .varPath = "var1", .typePath = "Var1"},
      {.isMutable = false, .varPath = ""},
      {.isMutable = false, .varPath = "var2", .typePath = "foo::bar::Var2"},
      {.isMutable = false, .varPath = "", .typePath = "Int"},
      {.isMutable = false, .typePath = "Float"}
    }
  );
}

TEST_CASE_WITH_FIXTURE("Parser: array", "[Parser]") {
  static constexpr auto assertArray = []<IsStrictlyASTNode ...T>(String source) {
    const auto array = parseNoThrow<ArrayExpr>(std::move(source), CombParser::arrayExpr());

    REQUIRE(sizeof...(T) == array.size());

    static const auto assertElement = [&]<typename U>() -> void {
      static sz index = 0;
      REQUIRE(matchAstType<U>(array.element(index++)));
    };

    (assertElement.template operator()<T>(), ...);
  };

  // assertArray.operator()<
  //   BooleanLiteral, BooleanLiteral, VarId, BooleanLiteral
  // >("[true, false, x, true] ");

  // []
  // [true, false, x, true]
  // [true, false, false, true]
}

TEST_CASE_WITH_FIXTURE("Parser: tuple", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: lambda", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: primary expression", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: function application", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: subscript", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: access", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: postfix expression", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: prefix operators", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: binary operators", "[Parser]") {
}

TEST_CASE_WITH_FIXTURE("Parser: ternary operators", "[Parser]") {
}
