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
        "type::Identifier [@0:0] with (fundamental, path) = (true, 'Int')"
    );
    assertType(
        "foo::bar::Baz",
        "type::Identifier [@0:0] with (fundamental, path) = (false, 'foo::bar::Baz')"
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
        "├─ type::Identifier [@0:1] with (fundamental, path) = (false, 'Foo')"
    );
    assertType(
        "(Int, Bool)",
        "type::Tuple [@0:0] with size = 2\n"
        "├─ type::Identifier [@0:1] with (fundamental, path) = (true, 'Int')\n"
        "├─ type::Identifier [@0:6] with (fundamental, path) = (true, 'Bool')"
    );
    assertType(
        "(Int, Float, Bool, foo::Bar)",
        "type::Tuple [@0:0] with size = 4\n"
        "├─ type::Identifier [@0:1] with (fundamental, path) = (true, 'Int')\n"
        "├─ type::Identifier [@0:6] with (fundamental, path) = (true, 'Float')\n"
        "├─ type::Identifier [@0:13] with (fundamental, path) = (true, 'Bool')\n"
        "├─ type::Identifier [@0:19] with (fundamental, path) = (false, 'foo::Bar')"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Array types", "[Parse]") {
    assertType(
        "Int[]",
        "type::Array [@0:0] with dims = {0:dyn}\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Int')"
    );
    assertType(
        "[[x]][5][]",
        "type::Array [@0:0] with dims = {0:fix,1:dyn}\n"
        "├─ type::Infer [@0:0]\n"
        "   ├─ expr::Identifier [@0:2] with path = 'x'\n"
        "├─ expr::Integer [@0:6] with value = 5"
    );
    assertType(
        "foo::Bar[][x+1][][]",
        "type::Array [@0:0] with dims = {0:dyn,1:fix,2:dyn,3:dyn}\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (false, 'foo::Bar')\n"
        "├─ expr::Binary [@0:11] with op = '+'\n"
        "   ├─ expr::Identifier [@0:11] with path = 'x'\n"
        "   ├─ expr::Integer [@0:13] with value = 1"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function types", "[Parse]") {
    assertType(
        "() -> Void",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 0\n"
        "├─ type::Identifier [@0:6] with (fundamental, path) = (true, 'Void')"
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
        "├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Int')\n"
        "├─ type::Identifier [@0:7] with (fundamental, path) = (true, 'Bool')"
    );
    assertType(
        "(Int, Float) -> foo::Bar",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 2\n"
        "   ├─ type::Identifier [@0:1] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ type::Identifier [@0:6] with (fundamental, path) = (true, 'Float')\n"
        "├─ type::Identifier [@0:16] with (fundamental, path) = (false, 'foo::Bar')"
    );
    assertType(
        "(Int, foo::Bar) -> (Int, Bool)",
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 2\n"
        "   ├─ type::Identifier [@0:1] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ type::Identifier [@0:6] with (fundamental, path) = (false, 'foo::Bar')\n"
        "├─ type::Tuple [@0:19] with size = 2\n"
        "   ├─ type::Identifier [@0:20] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ type::Identifier [@0:25] with (fundamental, path) = (true, 'Bool')"
    );
    // assertType(
    //     "Int -> Float -> Bool",
    //     ""
    // );
}

TEST_CASE_WITH_FIXTURE("Parse: Generic types", "[Parse]") {
    assertType(
        "Own<foo::Bar>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Own')\n"
        "├─ type::Identifier [@0:4] with (fundamental, path) = (false, 'foo::Bar')"
    );
    assertType(
        "Ref<Int>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Ref')\n"
        "├─ type::Identifier [@0:4] with (fundamental, path) = (true, 'Int')"
    );
    assertType(
        "foo::Bar<Int, Float, Bool>",
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (fundamental, path) = (false, 'foo::Bar')\n"
        "├─ type::Identifier [@0:9] with (fundamental, path) = (true, 'Int')\n"
        "├─ type::Identifier [@0:14] with (fundamental, path) = (true, 'Float')\n"
        "├─ type::Identifier [@0:21] with (fundamental, path) = (true, 'Bool')"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Binary type-expressions", "[Parse]") {
    assertType(
        "Int | (Float, Int) | foo::Bar",
        "type::Binary [@0:0] with op = '|'\n"
        "├─ type::Binary [@0:0] with op = '|'\n"
        "   ├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ type::Tuple [@0:6] with size = 2\n"
        "      ├─ type::Identifier [@0:7] with (fundamental, path) = (true, 'Float')\n"
        "      ├─ type::Identifier [@0:14] with (fundamental, path) = (true, 'Int')\n"
        "├─ type::Identifier [@0:21] with (fundamental, path) = (false, 'foo::Bar')"
    );
    assertType(
        "Int | foo::Bar<Int, Float> | foo::Baz",
        "type::Binary [@0:0] with op = '|'\n"
        "├─ type::Binary [@0:0] with op = '|'\n"
        "   ├─ type::Identifier [@0:0] with (fundamental, path) = (true, 'Int')\n"
        "   ├─ type::Generic [@0:6]\n"
        "      ├─ type::Identifier [@0:6] with (fundamental, path) = (false, 'foo::Bar')\n"
        "      ├─ type::Identifier [@0:15] with (fundamental, path) = (true, 'Int')\n"
        "      ├─ type::Identifier [@0:20] with (fundamental, path) = (true, 'Float')\n"
        "├─ type::Identifier [@0:29] with (fundamental, path) = (false, 'foo::Baz')"
    );
}
