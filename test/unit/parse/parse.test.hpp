#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>
#include <source_location>

#include "parse/parse.hpp"

using namespace tlc::syntax;
using Context = tlc::parse::EParseErrorContext;
using Reason = tlc::parse::EParseErrorReason;

class ParseTestFixture {
protected:
    using FnNode = void (*)(Node const&);
    using FnNodes = void (*)(tlc::Span<Node const>);
    template <IsASTNode T>
    using FnNodeT = void (*)(T const&);
    using Error = tlc::Error<Context, Reason>;
    using ErrorParams = Error::Params;
    using ErrCollector =
    tlc::ErrorCollector<Context, Reason>;
    using SLoc = std::source_location;

    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/parse.toy";

protected:
    struct AssertParams {
        tlc::Str source;
        tlc::Opt<tlc::Str> expectedAstPrint;
        tlc::Opt<tlc::Str> expectedPrettyPrint;
        tlc::Vec<ErrorParams> expectedErrors;
    };

protected:
    template <IsASTNode T>
    static auto cast(Node const& node) -> T {
        REQUIRE(matchAstType<T>(node));
        return astCast<T>(node);
    }

    static auto assertExpr(
        AssertParams params, SLoc location = SLoc::current()
    ) -> void;

    // static auto assertString(
    //     tlc::Str source, tlc::Str expected, tlc::Vec<tlc::Str> expectedFragments,
    //     SLoc location = SLoc::current()
    // ) -> void;

    static auto assertType(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    // static auto assertDecl(
    //     tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    // ) -> void;
    //
    // static auto assertGenericParamsDecl(
    //     tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    // ) -> void;
    //
    // static auto assertStmt(
    //     tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    // ) -> void;
    //
    // static auto assertTranslationUnit(
    //     tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    // ) -> void;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
