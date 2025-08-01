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
        "   ├─ decl::Identifier [@0:11] with (const, name) = (true, 'z')\n"
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
        "stmt::Assign [@0:0] with op = '='\n"
        "├─ expr::Identifier [@0:0] with path = 'x'\n"
        "├─ expr::Integer [@0:4] with value = 5"
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
        "x == y => foo::Bar{sum: x+y, product: x*y};",
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Expression [@0:10]\n"
        "   ├─ expr::Record [@0:10] with keys = ['sum','product']\n"
        "      ├─ type::Identifier [@0:10] with (fundamental, path) = (false, 'foo::Bar')\n"
        "      ├─ expr::Binary [@0:24] with op = '+'\n"
        "         ├─ expr::Identifier [@0:24] with path = 'x'\n"
        "         ├─ expr::Identifier [@0:26] with path = 'y'\n"
        "      ├─ expr::Binary [@0:38] with op = '*'\n"
        "         ├─ expr::Identifier [@0:38] with path = 'x'\n"
        "         ├─ expr::Identifier [@0:40] with path = 'y'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Block statements", "[Parse]") {
    assertStmt(
        "{}",
        "stmt::Block [@0:0] with size = 0"
    );
    assertStmt(
        "{\n"
        "    let x = 3.14159;\n"
        "    yield x + y;\n"
        "}",
        "stmt::Block [@0:0] with size = 2\n"
        "├─ stmt::Let [@1:4]\n"
        "   ├─ decl::Identifier [@1:8] with (const, name) = (true, 'x')\n"
        "   ├─ expr::Float [@1:12] with value = 3.14159\n"
        "├─ stmt::Yield [@2:4]\n"
        "   ├─ expr::Binary [@2:10] with op = '+'\n"
        "      ├─ expr::Identifier [@2:10] with path = 'x'\n"
        "      ├─ expr::Identifier [@2:14] with path = 'y'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Defer and Preface statements", "[Parse]") {
    assertStmt(
        "preface x == 0 => return 0;",
        "stmt::Preface [@0:0]\n"
        "├─ stmt::Conditional [@0:8]\n"
        "   ├─ expr::Binary [@0:8] with op = '=='\n"
        "      ├─ expr::Identifier [@0:8] with path = 'x'\n"
        "      ├─ expr::Integer [@0:13] with value = 0\n"
        "   ├─ stmt::Return [@0:18]\n"
        "      ├─ expr::Integer [@0:25] with value = 0"
    );
    assertStmt(
        "defer io::println(x)",
        "stmt::Defer [@0:0]\n"
        "├─ stmt::Expression [@0:6]\n"
        "   ├─ expr::FnApp [@0:6]\n"
        "      ├─ expr::Identifier [@0:6] with path = 'io::println'\n"
        "      ├─ expr::Tuple [@0:17] with size = 1\n"
        "         ├─ expr::Identifier [@0:18] with path = 'x'"
    );
    assertStmt(
        "preface {}",
        "stmt::Preface [@0:0]\n"
        "├─ stmt::Block [@0:8] with size = 0"
    );
    assertStmt(
        "defer {}",
        "stmt::Defer [@0:0]\n"
        "├─ stmt::Block [@0:6] with size = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Loop statements", "[Parse]") {
    assertStmt(
        "for e in r0 {}",
        "stmt::Loop [@0:0]\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (true, 'e')\n"
        "├─ expr::Identifier [@0:9] with path = 'r0'\n"
        "├─ stmt::Block [@0:12] with size = 0"
    );
    assertStmt(
        "for (i: Int, v: Float) in r0 {}",
        "stmt::Loop [@0:0]\n"
        "├─ decl::Tuple [@0:4] with size = 2\n"
        "   ├─ decl::Identifier [@0:5] with (const, name) = (true, 'i')\n"
        "      ├─ type::Identifier [@0:8] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ decl::Identifier [@0:13] with (const, name) = (true, 'v')\n"
        "      ├─ type::Identifier [@0:16] with (fundamental, path) = (true, 'Float')\n"
        "├─ expr::Identifier [@0:26] with path = 'r0'\n"
        "├─ stmt::Block [@0:29] with size = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Match statements", "[Parse]") {
    assertStmt(
        "match x {\n"
        "    5 => y = x * 5;\n"
        "    when x < 5 => {}\n"
        "    _ => {}\n"
        "}",
        "stmt::Match [@0:0]\n"
        "├─ expr::Identifier [@0:6] with path = 'x'\n"
        "├─ stmt::MatchCase [@1:4]\n"
        "   ├─ expr::Integer [@1:4] with value = 5\n"
        "   ├─ stmt::Assign [@1:9] with op = '='\n"
        "      ├─ expr::Identifier [@1:9] with path = 'y'\n"
        "      ├─ expr::Binary [@1:13] with op = '*'\n"
        "         ├─ expr::Identifier [@1:13] with path = 'x'\n"
        "         ├─ expr::Integer [@1:17] with value = 5\n"
        "├─ stmt::MatchCase [@2:4]\n"
        "   ├─ expr::Binary [@2:9] with op = '<'\n"
        "      ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "      ├─ expr::Integer [@2:13] with value = 5\n"
        "   ├─ stmt::Block [@2:18] with size = 0\n"
        "├─ stmt::Block [@3:9] with size = 0"
    );
    assertStmt(
        "match (x,y) {\n"
        "    (_, 5) => z = x + y;\n"
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
        "   ├─ stmt::Assign [@1:14] with op = '='\n"
        "      ├─ expr::Identifier [@1:14] with path = 'z'\n"
        "      ├─ expr::Binary [@1:18] with op = '+'\n"
        "         ├─ expr::Identifier [@1:18] with path = 'x'\n"
        "         ├─ expr::Identifier [@1:22] with path = 'y'\n"
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
