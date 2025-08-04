#include "parse.test.hpp"

using namespace tlc::syntax;

auto ParseTestFixture::assertType(
    tlc::Str source, tlc::Str expected, std::source_location const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseType();
    REQUIRE(result.has_value());

    auto const actual = tlc::parse::ASTPrinter::operator()(
        std::move(*result)
    );
    REQUIRE(actual == expected);
}

TEST_CASE_WITH_FIXTURE("Parse: Type identifiers", "[Parse]") {
    assertType(
        "Int",
        "type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')"
    );
    assertType(
        "$foo.bar.Baz",
        "type::Identifier [@0:0] with (const, fund, path) = (false, false, 'foo.bar.Baz')"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Type inference operator", "[Parse]") {
    assertType(
        "[[x]]",
        "type::Infer [@0:0]\n"
        "├─ expr::Identifier [@0:2] with path = 'x'"
    );
    assertType(
        "[[3.14159]]",
        "type::Infer [@0:0]\n"
        "├─ expr::Float [@0:2] with value = 3.14159"
    );
    assertType(
        "[[x+2]]",
        "type::Infer [@0:0]\n"
        "├─ expr::Binary [@0:2] with op = '+'\n"
        "   ├─ expr::Identifier [@0:2] with path = 'x'\n"
        "   ├─ expr::Integer [@0:4] with value = 2"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Tuple types", "[Parse]") {
    assertType(
        "()",
        "type::Tuple [@0:0] with size = 0"
    );
    assertType(
        "(Foo)",
        "type::Tuple [@0:0] with size = 1\n"
        "├─ type::Identifier [@0:1] with (const, fund, path) = (true, false, 'Foo')"
    );
    assertType(
        "(Int, $Bool)",
        "type::Tuple [@0:0] with size = 2\n"
        "├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Identifier [@0:6] with (const, fund, path) = (false, true, 'Bool')"
    );
    assertType(
        "(Int, Float, Bool, $foo.Bar)",
        "type::Tuple [@0:0] with size = 4\n"
        "├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Identifier [@0:6] with (const, fund, path) = (true, true, 'Float')\n"
        "├─ type::Identifier [@0:13] with (const, fund, path) = (true, true, 'Bool')\n"
        "├─ type::Identifier [@0:19] with (const, fund, path) = (false, false, 'foo.Bar')"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Array types", "[Parse]") {
    assertType(
        "Int[5]",
        "type::Array [@0:0] with nDims = 1\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ expr::Integer [@0:4] with value = 5"
    );
    assertType(
        "[[x]][5, y+z]",
        "type::Array [@0:0] with nDims = 2\n"
        "├─ type::Infer [@0:0]\n"
        "   ├─ expr::Identifier [@0:2] with path = 'x'\n"
        "├─ expr::Integer [@0:6] with value = 5\n"
        "├─ expr::Binary [@0:9] with op = '+'\n"
        "   ├─ expr::Identifier [@0:9] with path = 'y'\n"
        "   ├─ expr::Identifier [@0:11] with path = 'z'"
    );
    assertType(
        "foo.Bar[3, x+1, 1, 4]",
        "type::Array [@0:0] with nDims = 4\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ expr::Integer [@0:8] with value = 3\n"
        "├─ expr::Binary [@0:11] with op = '+'\n"
        "   ├─ expr::Identifier [@0:11] with path = 'x'\n"
        "   ├─ expr::Integer [@0:13] with value = 1\n"
        "├─ expr::Integer [@0:16] with value = 1\n"
        "├─ expr::Integer [@0:19] with value = 4"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function types", "[Parse]") {
    assertType(
        "() -> Void",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 0\n"
        "├─ type::Identifier [@0:6] with (const, fund, path) = (true, true, 'Void')"
    );
    assertType(
        "() -> ()",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 0\n"
        "├─ type::Tuple [@0:6] with size = 0"
    );
    assertType(
        "Int -> Bool",
        "type::Function [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Identifier [@0:7] with (const, fund, path) = (true, true, 'Bool')"
    );
    assertType(
        "(Int, Float) -> foo.Bar",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 2\n"
        "   ├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:6] with (const, fund, path) = (true, true, 'Float')\n"
        "├─ type::Identifier [@0:16] with (const, fund, path) = (true, false, 'foo.Bar')"
    );
    assertType(
        "(Int, foo.Bar) -> (Int, Bool)",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 2\n"
        "   ├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:6] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ type::Tuple [@0:18] with size = 2\n"
        "   ├─ type::Identifier [@0:19] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:24] with (const, fund, path) = (true, true, 'Bool')"
    );
    assertType(
        "Int -> Float -> Bool -> foo.Bar",
        "type::Function [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Function [@0:7]\n"
        "   ├─ type::Identifier [@0:7] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Function [@0:16]\n"
        "      ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Bool')\n"
        "      ├─ type::Identifier [@0:24] with (const, fund, path) = (true, false, 'foo.Bar')"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Generic types", "[Parse]") {
    assertType(
        "Own<foo.Bar>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Own')\n"
        "├─ type::GenericArguments [@0:3] with size = 1\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, false, 'foo.Bar')"
    );
    assertType(
        "Ref<Int>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Ref')\n"
        "├─ type::GenericArguments [@0:3] with size = 1\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Int')"
    );
    assertType(
        "foo.Bar<Int, Float, Bool>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ type::GenericArguments [@0:7] with size = 3\n"
        "   ├─ type::Identifier [@0:8] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:13] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Identifier [@0:20] with (const, fund, path) = (true, true, 'Bool')"
    );
    assertType(
        "foo.Bar<foo.Baz<Int, Float>, Bool>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ type::GenericArguments [@0:7] with size = 2\n"
        "   ├─ type::Generic [@0:8]\n"
        "      ├─ type::Identifier [@0:8] with (const, fund, path) = (true, false, 'foo.Baz')\n"
        "      ├─ type::GenericArguments [@0:15] with size = 2\n"
        "         ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Int')\n"
        "         ├─ type::Identifier [@0:21] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Identifier [@0:29] with (const, fund, path) = (true, true, 'Bool')"
    );
}
