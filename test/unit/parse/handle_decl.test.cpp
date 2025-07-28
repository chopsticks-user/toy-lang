#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

auto ParseTestFixture::assertDecl(
    tlc::Str source, tlc::Str expected, std::source_location const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseDecl();
    REQUIRE(result.has_value());

    auto const actual = tlc::parse::ASTPrinter::operator()(
        std::move(*result)
    );
    REQUIRE(actual == expected);
}

TEST_CASE_WITH_FIXTURE("Parse: Identifier decl", "[Parse]") {
    assertDecl(
        "x: Foo",
        "decl::Identifier [@0:3] with (const, name) = (true, 'x')\n" // todo: wrong coords
        "├─ type::Identifier [@0:3] with (fundamental, path) = (false, 'Foo')"
    );
    assertDecl(
        "$x: Float",
        "decl::Identifier [@0:4] with (const, name) = (false, 'x')\n" // todo: wrong coords
        "├─ type::Identifier [@0:4] with (fundamental, path) = (true, 'Float')"
    );
    assertDecl(
        "$x",
        "decl::Identifier [@0:0] with (const, name) = (false, 'x')" // todo: wrong coords
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Tuple decl", "[Parse]") {
    assertDecl(
        "()",
        "decl::Tuple [@0:0] with size = 0"
    );
    assertDecl(
        "($x)",
        "decl::Tuple [@0:0] with size = 1\n"
        "├─ decl::Identifier [@0:1] with (const, name) = (false, 'x')"
    );
    assertDecl(
        "(x: Int, $y: Float, z)",
        "decl::Tuple [@0:0] with size = 3\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (true, 'x')\n"
        "   ├─ type::Identifier [@0:4] with (fundamental, path) = (true, 'Int')\n"
        "├─ decl::Identifier [@0:13] with (const, name) = (false, 'y')\n"
        "   ├─ type::Identifier [@0:13] with (fundamental, path) = (true, 'Float')\n"
        "├─ decl::Identifier [@0:20] with (const, name) = (true, 'z')"
    );
    assertDecl(
        "(x: Int, $y: Float, ($z, t))",
        "decl::Tuple [@0:0] with size = 3\n"
        "├─ decl::Identifier [@0:4] with (const, name) = (true, 'x')\n"
        "   ├─ type::Identifier [@0:4] with (fundamental, path) = (true, 'Int')\n"
        "├─ decl::Identifier [@0:13] with (const, name) = (false, 'y')\n"
        "   ├─ type::Identifier [@0:13] with (fundamental, path) = (true, 'Float')\n"
        "├─ decl::Tuple [@0:20] with size = 2\n"
        "   ├─ decl::Identifier [@0:21] with (const, name) = (false, 'z')\n"
        "   ├─ decl::Identifier [@0:25] with (const, name) = (true, 't')"
    );
}
