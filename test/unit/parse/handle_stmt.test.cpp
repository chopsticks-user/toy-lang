#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

auto ParseTestFixture::assertStmt(
    tlc::Str source, tlc::Str expected, std::source_location const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseStmt();
    REQUIRE(result.has_value());

    auto const actual = tlc::parse::ASTPrinter::operator()(
        std::move(*result)
    );
    REQUIRE(actual == expected);
}

TEST_CASE_WITH_FIXTURE("Parse: Yield and Return statements", "[Parse]") {
    assertStmt(
        "return;",
        "stmt::Return [@0:0]"
    );
    assertStmt(
        "return x;",
        "stmt::Return [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'x'"
    );
    assertStmt(
        "return (foo::bar, 0);",
        "stmt::Return [@0:0]\n"
        "├─ expr::Tuple [@0:7] with size = 2\n"
        "   ├─ expr::Identifier [@0:8] with path = 'foo::bar'\n"
        "   ├─ expr::Integer [@0:18] with value = 0"
    );

    assertStmt(
        "yield x;",
        "stmt::Yield [@0:0]\n"
        "├─ expr::Identifier [@0:6] with path = 'x'"
    );
    assertStmt(
        "yield (foo::bar, 0);",
        "stmt::Yield [@0:0]\n"
        "├─ expr::Tuple [@0:6] with size = 2\n"
        "   ├─ expr::Identifier [@0:7] with path = 'foo::bar'\n"
        "   ├─ expr::Integer [@0:17] with value = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Let statements", "[Parse]") {
    assertStmt(
        "let $x;",
        "stmt::Let [@0:0]\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (false, 'x')"
    );
    assertStmt(
        "let x = foo::Bar{x: 0, y: true};",
        "stmt::Let [@0:0]\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (true, 'x')\n"
        "├─ expr::Record [@0:8] with keys = ['x','y']\n"
        "   ├─ type::Identifier [@0:8] with (fundamental, path) = (false, 'foo::Bar')\n"
        "   ├─ expr::Integer [@0:20] with value = 0\n"
        "   ├─ expr::Boolean [@0:26] with value = true"
    );
    assertStmt(
        "let (x, y, z: Bool) = (0, 0.0, false);",
        "stmt::Let [@0:0]\n"
        "├─ decl::Tuple [@0:4] with size = 3\n"
        "   ├─ decl::Identifier [@0:5] with (const, name) = (true, 'x')\n"
        "   ├─ decl::Identifier [@0:8] with (const, name) = (true, 'y')\n"
        "   ├─ decl::Identifier [@0:14] with (const, name) = (true, 'z')\n"
        "      ├─ type::Identifier [@0:14] with (fundamental, path) = (true, 'Bool')\n"
        "├─ expr::Tuple [@0:22] with size = 3\n"
        "   ├─ expr::Integer [@0:23] with value = 0\n"
        "   ├─ expr::Float [@0:26] with value = 0\n"
        "   ├─ expr::Boolean [@0:31] with value = false"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Expr-prefixed statements", "[Parse]") {
    assertStmt(
        "foo::bar();",
        "stmt::Expression [@0:0]\n"
        "├─ expr::FnApp [@0:0]\n"
        "   ├─ expr::Identifier [@0:0] with path = 'foo::bar'\n"
        "   ├─ expr::Tuple [@0:8] with size = 0"
    );
    assertStmt(
        "x = 5;",
        "stmt::Assign [@0:4] with op = '='\n" // todo: wrong coords
        "├─ expr::Identifier [@0:0] with path = 'x'\n"
        "├─ expr::Integer [@0:4] with value = 5"
    );
    assertStmt(
        "x += 5;",
        "stmt::Assign [@0:5] with op = '+='\n" // todo: wrong coords
        "├─ expr::Identifier [@0:0] with path = 'x'\n"
        "├─ expr::Integer [@0:5] with value = 5"
    );
    assertStmt(
        "x == y => return x + y;",
        "stmt::Conditional [@0:21]\n"
        "├─ expr::Binary [@0:5] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Return [@0:10]\n"
        "   ├─ expr::Binary [@0:21] with op = '+'\n"
        "      ├─ expr::Identifier [@0:17] with path = 'x'\n"
        "      ├─ expr::Identifier [@0:21] with path = 'y'"
    );
    assertStmt(
        "x == y => foo::Bar{sum: x+y, product: x*y};",
        "stmt::Conditional [@0:40]\n"
        "├─ expr::Binary [@0:5] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Expression [@0:40]\n"
        "   ├─ expr::Record [@0:10] with keys = ['sum','product']\n"
        "      ├─ type::Identifier [@0:10] with (fundamental, path) = (false, 'foo::Bar')\n"
        "      ├─ expr::Binary [@0:26] with op = '+'\n"
        "         ├─ expr::Identifier [@0:24] with path = 'x'\n"
        "         ├─ expr::Identifier [@0:26] with path = 'y'\n"
        "      ├─ expr::Binary [@0:40] with op = '*'\n"
        "         ├─ expr::Identifier [@0:38] with path = 'x'\n"
        "         ├─ expr::Identifier [@0:40] with path = 'y'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Block statements", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Loop statements", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Match statements", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Defer and Preface statements", "[Parse]") {}
