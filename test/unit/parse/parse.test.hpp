#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>
#include <source_location>

#include "parse/parse.hpp"

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

class ParseTestFixture {
protected:
    using FnNode = void (*)(tlc::syntax::Node const&);
    using FnNodes = void (*)(tlc::Span<tlc::syntax::Node const>);
    template <tlc::syntax::IsASTNode T>
    using FnNodeT = void (*)(T const&);

    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/parse.toy";

protected:
    template <tlc::syntax::IsASTNode T>
    static auto cast(tlc::syntax::Node const& node) -> T {
        REQUIRE(tlc::syntax::matchAstType<T>(node));
        return tlc::syntax::astCast<T>(node);
    }

    static auto assertExpr(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertString(
        tlc::Str source, tlc::Str expected, tlc::Vec<tlc::Str> expectedFragments,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertType(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertDecl(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertGenericParamsDecl(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertStmt(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertTranslationUnit(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;
};

inline auto ParseTestFixture::assertTranslationUnit(
    tlc::Str source, tlc::Str expected, std::source_location location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }();
    REQUIRE(matchAstType<tlc::syntax::TranslationUnit>(result));
    REQUIRE(astCast<tlc::syntax::TranslationUnit>(result).sourcePath()
        == filepath);

    auto const actual = tlc::parse::ASTPrinter::operator()(std::move(result));
    REQUIRE(actual == expected);
}

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
