#include "parse.test.hpp"

using namespace tlc::syntax;

auto ParseTestFixture::assertTranslationUnit(
    tlc::Str source, tlc::Str expected, std::source_location location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }();
    REQUIRE(matchAstType<TranslationUnit>(result));
    REQUIRE(astCast<TranslationUnit>(result).sourcePath() == filepath);

    auto const actual = tlc::parse::ASTPrinter::operator()(std::move(result));
    REQUIRE(actual == expected);
}

TEST_CASE_WITH_FIXTURE("Parse: Module and import declarations", "[Parse]") {
    assertTranslationUnit(
        "module",
        ""
    );
    assertTranslationUnit(
        "module foo",
        "├─ global::ModuleDecl [@0:0]\n"
        "   ├─ expr::Identifier [@0:7] with path = 'foo'"
    );
    assertTranslationUnit(
        "module foo.bar.baz",
        "├─ global::ModuleDecl [@0:0]\n"
        "   ├─ expr::Identifier [@0:7] with path = 'foo.bar.baz'"
    );
}
