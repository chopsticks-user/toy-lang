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

TEST_CASE_WITH_FIXTURE("Parse: Return statements", "[Parse]") {
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
}

TEST_CASE_WITH_FIXTURE("Parse: Let statements", "[Parse]") {
    assertStmt(
        "let $x;",
        "stmt::Let [@0:0]\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (false, 'x')"
    );

    assertStmt(
        "let x: foo::Bar = {x: 0, y: true};",
        "stmt::Let [@0:0]\n"
        "├─ decl::Identifier [@0:7] with (const, name) = (true, 'x')\n" // todo: wrong coords
        "   ├─ type::Identifier [@0:7] with (fundamental, path) = (false, 'foo::Bar')\n"
        "├─ expr::Record [@0:18] with keys = ['x','y']\n"
        "   ├─ expr::Integer [@0:22] with value = 0\n"
        "   ├─ expr::Boolean [@0:28] with value = true"
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
