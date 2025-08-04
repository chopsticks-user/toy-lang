#include "parse.test.hpp"

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
        "return (foo.bar, 0);",
        "stmt::Return [@0:0]\n"
        "├─ expr::Tuple [@0:7] with size = 2\n"
        "   ├─ expr::Identifier [@0:8] with path = 'foo.bar'\n"
        "   ├─ expr::Integer [@0:17] with value = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Declaration statements", "[Parse]") {
    assertStmt(
        "x = foo.Bar{x: 0, y: true};",
        "stmt::Decl [@0:0]\n"
        "├─ decl::Identifier [@0:0] with name = 'x'\n"
        "├─ expr::Record [@0:4] with size = 2\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "   ├─ expr::RecordEntry [@0:12] with key = 'x'\n"
        "      ├─ expr::Integer [@0:15] with value = 0\n"
        "   ├─ expr::RecordEntry [@0:18] with key = 'y'\n"
        "      ├─ expr::Boolean [@0:21] with value = true"
    );
    assertStmt(
        "(x, y, z: Bool) = (0, 0.0, false);",
        "stmt::Decl [@0:0]\n"
        "├─ decl::Tuple [@0:0] with size = 3\n"
        "   ├─ decl::Identifier [@0:1] with name = 'x'\n"
        "   ├─ decl::Identifier [@0:4] with name = 'y'\n"
        "   ├─ decl::Identifier [@0:7] with name = 'z'\n"
        "      ├─ type::Identifier [@0:10] with (const, fund, path) = (true, true, 'Bool')\n"
        "├─ expr::Tuple [@0:18] with size = 3\n"
        "   ├─ expr::Integer [@0:19] with value = 0\n"
        "   ├─ expr::Float [@0:22] with value = 0\n"
        "   ├─ expr::Boolean [@0:27] with value = false"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Expr-prefixed statements", "[Parse]") {
    assertStmt(
        "foo.bar();",
        "stmt::Expression [@0:0]\n"
        "├─ expr::FnApp [@0:0]\n"
        "   ├─ expr::Identifier [@0:0] with path = 'foo.bar'\n"
        "   ├─ expr::Tuple [@0:7] with size = 0"
    );
    assertStmt(
        "x := 5;",
        "stmt::Assign [@0:0] with op = ':='\n"
        "├─ expr::Identifier [@0:0] with path = 'x'\n"
        "├─ expr::Integer [@0:5] with value = 5"
    );
    assertStmt(
        "x += 5;",
        "stmt::Assign [@0:0] with op = '+='\n"
        "├─ expr::Identifier [@0:0] with path = 'x'\n"
        "├─ expr::Integer [@0:5] with value = 5"
    );
    assertStmt(
        "x == y => return x + y;",
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Return [@0:10]\n"
        "   ├─ expr::Binary [@0:17] with op = '+'\n"
        "      ├─ expr::Identifier [@0:17] with path = 'x'\n"
        "      ├─ expr::Identifier [@0:21] with path = 'y'"
    );
    assertStmt(
        "x == y => foo.Bar{sum: x+y, product: x*y};",
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Expression [@0:10]\n"
        "   ├─ expr::Record [@0:10] with size = 2\n"
        "      ├─ type::Identifier [@0:10] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "      ├─ expr::RecordEntry [@0:18] with key = 'sum'\n"
        "         ├─ expr::Binary [@0:23] with op = '+'\n"
        "            ├─ expr::Identifier [@0:23] with path = 'x'\n"
        "            ├─ expr::Identifier [@0:25] with path = 'y'\n"
        "      ├─ expr::RecordEntry [@0:28] with key = 'product'\n"
        "         ├─ expr::Binary [@0:37] with op = '*'\n"
        "            ├─ expr::Identifier [@0:37] with path = 'x'\n"
        "            ├─ expr::Identifier [@0:39] with path = 'y'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Block statements", "[Parse]") {
    assertStmt(
        "{}",
        "stmt::Block [@0:0] with size = 0"
    );
    assertStmt(
        "{\n"
        "    x = 3.14159;\n"
        "    x := x + y;\n"
        "}",
        "stmt::Block [@0:0] with size = 2\n"
        "├─ stmt::Decl [@1:4]\n"
        "   ├─ decl::Identifier [@1:4] with name = 'x'\n"
        "   ├─ expr::Float [@1:8] with value = 3.14159\n"
        "├─ stmt::Assign [@2:4] with op = ':='\n"
        "   ├─ expr::Identifier [@2:4] with path = 'x'\n"
        "   ├─ expr::Binary [@2:9] with op = '+'\n"
        "      ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "      ├─ expr::Identifier [@2:13] with path = 'y'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Defer statements", "[Parse]") {
    assertStmt(
        "defer io.println(x)",
        "stmt::Defer [@0:0]\n"
        "├─ stmt::Expression [@0:6]\n"
        "   ├─ expr::FnApp [@0:6]\n"
        "      ├─ expr::Identifier [@0:6] with path = 'io.println'\n"
        "      ├─ expr::Tuple [@0:16] with size = 1\n"
        "         ├─ expr::Identifier [@0:17] with path = 'x'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Loop statements", "[Parse]") {
    assertStmt(
        "for e in r0 {}",
        "stmt::Loop [@0:0]\n"
        "├─ decl::Identifier [@0:4] with name = 'e'\n"
        "├─ expr::Identifier [@0:9] with path = 'r0'\n"
        "├─ stmt::Block [@0:12] with size = 0"
    );
    assertStmt(
        "for (i: Int, v: Float) in r0 {}",
        "stmt::Loop [@0:0]\n"
        "├─ decl::Tuple [@0:4] with size = 2\n"
        "   ├─ decl::Identifier [@0:5] with name = 'i'\n"
        "      ├─ type::Identifier [@0:8] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ decl::Identifier [@0:13] with name = 'v'\n"
        "      ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Float')\n"
        "├─ expr::Identifier [@0:26] with path = 'r0'\n"
        "├─ stmt::Block [@0:29] with size = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Match statements", "[Parse]") {
    assertStmt(
        "match x {\n"
        "    5 => y := x * 5;\n"
        "    when x < 5 => {}\n"
        "    _ => {}\n"
        "}",
        "stmt::Match [@0:0]\n"
        "├─ expr::Identifier [@0:6] with path = 'x'\n"
        "├─ stmt::MatchCase [@1:4]\n"
        "   ├─ expr::Integer [@1:4] with value = 5\n"
        "   ├─ stmt::Assign [@1:9] with op = ':='\n"
        "      ├─ expr::Identifier [@1:9] with path = 'y'\n"
        "      ├─ expr::Binary [@1:14] with op = '*'\n"
        "         ├─ expr::Identifier [@1:14] with path = 'x'\n"
        "         ├─ expr::Integer [@1:18] with value = 5\n"
        "├─ stmt::MatchCase [@2:4]\n"
        "   ├─ expr::Binary [@2:9] with op = '<'\n"
        "      ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "      ├─ expr::Integer [@2:13] with value = 5\n"
        "   ├─ stmt::Block [@2:18] with size = 0\n"
        "├─ stmt::Block [@3:9] with size = 0"
    );
    assertStmt(
        "match (x,y) {\n"
        "    (_, 5) => z := x + y;\n"
        "    when x < 5 && y > 0 => {}\n"
        "    (10, _) when y <= 0 => {}\n"
        "    _ => {}\n"
        "}",
        "stmt::Match [@0:0]\n"
        "├─ expr::Tuple [@0:6] with size = 2\n"
        "   ├─ expr::Identifier [@0:7] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:9] with path = 'y'\n"
        "├─ stmt::MatchCase [@1:4]\n"
        "   ├─ expr::Tuple [@1:4] with size = 2\n"
        "      ├─ expr::Identifier [@1:5] with path = '_'\n"
        "      ├─ expr::Integer [@1:8] with value = 5\n"
        "   ├─ stmt::Assign [@1:14] with op = ':='\n"
        "      ├─ expr::Identifier [@1:14] with path = 'z'\n"
        "      ├─ expr::Binary [@1:19] with op = '+'\n"
        "         ├─ expr::Identifier [@1:19] with path = 'x'\n"
        "         ├─ expr::Identifier [@1:23] with path = 'y'\n"
        "├─ stmt::MatchCase [@2:4]\n"
        "   ├─ expr::Binary [@2:9] with op = '&&'\n"
        "      ├─ expr::Binary [@2:9] with op = '<'\n"
        "         ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "         ├─ expr::Integer [@2:13] with value = 5\n"
        "      ├─ expr::Binary [@2:18] with op = '>'\n"
        "         ├─ expr::Identifier [@2:18] with path = 'y'\n"
        "         ├─ expr::Integer [@2:22] with value = 0\n"
        "   ├─ stmt::Block [@2:27] with size = 0\n"
        "├─ stmt::MatchCase [@3:4]\n"
        "   ├─ expr::Tuple [@3:4] with size = 2\n"
        "      ├─ expr::Integer [@3:5] with value = 10\n"
        "      ├─ expr::Identifier [@3:9] with path = '_'\n"
        "   ├─ expr::Binary [@3:17] with op = '<='\n"
        "      ├─ expr::Identifier [@3:17] with path = 'y'\n"
        "      ├─ expr::Integer [@3:22] with value = 0\n"
        "   ├─ stmt::Block [@3:27] with size = 0\n"
        "├─ stmt::Block [@4:9] with size = 0"
    );
}
