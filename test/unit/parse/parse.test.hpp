#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>
#include <source_location>

#include "parse/parse.hpp"

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

    static auto assertType(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertDecl(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;

    static auto assertStmt(
        tlc::Str source, tlc::Str expected,
        std::source_location location = std::source_location::current()
    ) -> void;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
