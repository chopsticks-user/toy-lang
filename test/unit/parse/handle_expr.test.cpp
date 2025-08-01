#include "parse.test.hpp"

using namespace tlc::syntax;

auto ParseTestFixture::assertExpr(
    tlc::Str source, tlc::Str expected, std::source_location const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseExpr();
    REQUIRE(result.has_value());

    auto const actual = tlc::parse::ASTPrinter::operator()(
        std::move(*result)
    );
    REQUIRE(actual == expected);
}

TEST_CASE_WITH_FIXTURE("Parse: Integers", "[Parse]") {
    assertExpr(
        "31415",
        "expr::Integer [@0:0] with value = 31415"
    );
    assertExpr(
        "0",
        "expr::Integer [@0:0] with value = 0"
    );
    assertExpr(
        "0x123456789abcdef",
        "expr::Integer [@0:0] with value = 81985529216486895"
    );
    assertExpr(
        "0x0",
        "expr::Integer [@0:0] with value = 0"
    );
    assertExpr(
        "01234567",
        "expr::Integer [@0:0] with value = 342391"
    );
    assertExpr(
        "00",
        "expr::Integer [@0:0] with value = 0"
    );
    assertExpr(
        "0b1010101001",
        "expr::Integer [@0:0] with value = 681"
    );
    assertExpr(
        "0b0",
        "expr::Integer [@0:0] with value = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Floats", "[Parse]") {
    assertExpr(
        "0.0314159",
        "expr::Float [@0:0] with value = 0.0314159"
    );
    assertExpr(
        "3145.1926",
        "expr::Float [@0:0] with value = 3145.1926"
    );
    assertExpr(
        "0.314159",
        "expr::Float [@0:0] with value = 0.314159"
    );
    assertExpr(
        "00.31415",
        "expr::Float [@0:0] with value = 0.31415"
    );
    assertExpr(
        "000.3141",
        "expr::Float [@0:0] with value = 0.3141"
    );
    assertExpr(
        "0.000",
        "expr::Float [@0:0] with value = 0"
    );
    assertExpr(
        "00.00",
        "expr::Float [@0:0] with value = 0"
    );
    assertExpr(
        "000.0",
        "expr::Float [@0:0] with value = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Booleans", "[Parse]") {
    assertExpr(
        "true",
        "expr::Boolean [@0:0] with value = true"
    );
    assertExpr(
        "false",
        "expr::Boolean [@0:0] with value = false"
    );
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    assertExpr(
        "baz",
        "expr::Identifier [@0:0] with path = 'baz'"
    );
    assertExpr(
        "_",
        "expr::Identifier [@0:0] with path = '_'"
    );
    assertExpr(
        "foo::bar",
        "expr::Identifier [@0:0] with path = 'foo::bar'"
    );
    assertExpr(
        "foo::bar::baz::boo",
        "expr::Identifier [@0:0] with path = 'foo::bar::baz::boo'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Tuples", "[Parse]") {
    assertExpr(
        "(1,x,2.1)",
        "expr::Tuple [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ expr::Identifier [@0:3] with path = 'x'\n"
        "├─ expr::Float [@0:5] with value = 2.1"
    );
    assertExpr(
        "(x)",
        "expr::Tuple [@0:0] with size = 1\n"
        "├─ expr::Identifier [@0:1] with path = 'x'"
    );
    assertExpr(
        "()",
        "expr::Tuple [@0:0] with size = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Arrays", "[Parse]") {
    assertExpr(
        "[1,x,2.1]",
        "expr::Array [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ expr::Identifier [@0:3] with path = 'x'\n"
        "├─ expr::Float [@0:5] with value = 2.1"
    );
    assertExpr(
        "[0]",
        "expr::Array [@0:0] with size = 1\n"
        "├─ expr::Integer [@0:1] with value = 0"
    );
    assertExpr(
        "[]",
        "expr::Array [@0:0] with size = 0"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Records", "[Parse]") {
    assertExpr(
        "foo::Bar{}",
        "expr::Record [@0:0] with keys = []\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (false, 'foo::Bar')"
    );

    assertExpr(
        "Foo{x: 0, y: 1.1, z: false}\n",
        "expr::Record [@0:0] with keys = ['x','y','z']\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (false, 'Foo')\n"
        "├─ expr::Integer [@0:7] with value = 0\n"
        "├─ expr::Float [@0:13] with value = 1.1\n"
        "├─ expr::Boolean [@0:21] with value = false"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Access expressions", "[Parse]") {
    assertExpr(
        "foo.bar",
        "expr::Access [@0:0] with field = 'bar'\n"
        "├─ expr::Identifier [@0:0] with path = 'foo'"
    );
    assertExpr(
        "tmp::foo.bar.baz",
        "expr::Access [@0:0] with field = 'baz'\n"
        "├─ expr::Access [@0:0] with field = 'bar'\n"
        "   ├─ expr::Identifier [@0:0] with path = 'tmp::foo'"
    );
    assertExpr(
        "[0,1,2].foo",
        "expr::Access [@0:0] with field = 'foo'\n"
        "├─ expr::Array [@0:0] with size = 3\n"
        "   ├─ expr::Integer [@0:1] with value = 0\n"
        "   ├─ expr::Integer [@0:3] with value = 1\n"
        "   ├─ expr::Integer [@0:5] with value = 2"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Parse]") {
    assertExpr(
        "foo::bar(0,1,2)",
        "expr::FnApp [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo::bar'\n"
        "├─ expr::Tuple [@0:8] with size = 3\n"
        "   ├─ expr::Integer [@0:9] with value = 0\n"
        "   ├─ expr::Integer [@0:11] with value = 1\n"
        "   ├─ expr::Integer [@0:13] with value = 2"
    );
    assertExpr(
        "foo()",
        "expr::FnApp [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "├─ expr::Tuple [@0:3] with size = 0"
    );
    assertExpr(
        "foo(0,1,2)(bar)",
        "expr::FnApp [@0:0]\n"
        "├─ expr::FnApp [@0:0]\n"
        "   ├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "   ├─ expr::Tuple [@0:3] with size = 3\n"
        "      ├─ expr::Integer [@0:4] with value = 0\n"
        "      ├─ expr::Integer [@0:6] with value = 1\n"
        "      ├─ expr::Integer [@0:8] with value = 2\n"
        "├─ expr::Tuple [@0:10] with size = 1\n"
        "   ├─ expr::Identifier [@0:11] with path = 'bar'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Parse]") {
    assertExpr(
        "foo::bar[0,1,2]",
        "expr::Subscript [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo::bar'\n"
        "├─ expr::Array [@0:8] with size = 3\n"
        "   ├─ expr::Integer [@0:9] with value = 0\n"
        "   ├─ expr::Integer [@0:11] with value = 1\n"
        "   ├─ expr::Integer [@0:13] with value = 2"
    );
    assertExpr(
        "foo[x]",
        "expr::Subscript [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "├─ expr::Array [@0:3] with size = 1\n"
        "   ├─ expr::Identifier [@0:4] with path = 'x'"
    );
    assertExpr(
        "foo[0,1,2][bar]",
        "expr::Subscript [@0:0]\n"
        "├─ expr::Subscript [@0:0]\n"
        "   ├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "   ├─ expr::Array [@0:3] with size = 3\n"
        "      ├─ expr::Integer [@0:4] with value = 0\n"
        "      ├─ expr::Integer [@0:6] with value = 1\n"
        "      ├─ expr::Integer [@0:8] with value = 2\n"
        "├─ expr::Array [@0:10] with size = 1\n"
        "   ├─ expr::Identifier [@0:11] with path = 'bar'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Prefix expressions", "[Parse]") {
    assertExpr(
        "!x",
        "expr::Prefix [@0:0] with op = '!'\n"
        "├─ expr::Identifier [@0:1] with path = 'x'"
    );
    assertExpr(
        "!!!x",
        "expr::Prefix [@0:0] with op = '!'\n"
        "├─ expr::Prefix [@0:1] with op = '!'\n"
        "   ├─ expr::Prefix [@0:2] with op = '!'\n"
        "      ├─ expr::Identifier [@0:3] with path = 'x'"
    );
    assertExpr(
        "...[0,1,2]",
        "expr::Prefix [@0:0] with op = '...'\n"
        "├─ expr::Array [@0:3] with size = 3\n"
        "   ├─ expr::Integer [@0:4] with value = 0\n"
        "   ├─ expr::Integer [@0:6] with value = 1\n"
        "   ├─ expr::Integer [@0:8] with value = 2"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Binary expressions", "[Parse]") {
    assertExpr(
        "-x+3",
        "expr::Binary [@0:0] with op = '+'\n"
        "├─ expr::Prefix [@0:0] with op = '-'\n"
        "   ├─ expr::Identifier [@0:1] with path = 'x'\n"
        "├─ expr::Integer [@0:3] with value = 3"
    );
    assertExpr(
        "-x+y*z",
        "expr::Binary [@0:0] with op = '+'\n"
        "├─ expr::Prefix [@0:0] with op = '-'\n"
        "   ├─ expr::Identifier [@0:1] with path = 'x'\n"
        "├─ expr::Binary [@0:3] with op = '*'\n"
        "   ├─ expr::Identifier [@0:3] with path = 'y'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'z'"
    );
    assertExpr(
        "x*-y+z",
        "expr::Binary [@0:0] with op = '+'\n"
        "├─ expr::Binary [@0:0] with op = '*'\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Prefix [@0:2] with op = '-'\n"
        "      ├─ expr::Identifier [@0:3] with path = 'y'\n"
        "├─ expr::Identifier [@0:5] with path = 'z'"
    );
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Operator precedence", "[Parse]") {}
