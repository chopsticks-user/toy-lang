#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>
// #include <catch2/generators/catch_generators.hpp>
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
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertString(
        tlc::Str source, tlc::Str expected, tlc::Vec<tlc::Str> expectedFragments,
        SLoc location = SLoc::current()
    ) -> void;

    static auto assertType(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertDecl(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertGenericParamsDecl(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertStmt(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertTranslationUnit(
        tlc::Str source, tlc::Str expected, SLoc location = SLoc::current()
    ) -> void;

    static auto assertExprWithParams(
        AssertParams params, SLoc location = SLoc::current()
    ) -> void;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

inline auto ParseTestFixture::assertExprWithParams(
    AssertParams params, SLoc location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(params.source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseExpr();
    REQUIRE(result);

    params.expectedAstPrint.transform([&](auto&& expectedAstPrint) {
        auto const actualAstPrint = tlc::parse::ASTPrinter::operator()(
            // todo
            std::move(*result)
        );
        REQUIRE(actualAstPrint == expectedAstPrint);
        return "";
    });

    params.expectedPrettyPrint.transform(
        [&]([[maybe_unused]] auto&& expectedPrettyPrint) {
            return "";
        }
    );


    auto const actualErrors =
        ErrCollector::instance().errors();
    auto const expectedErrors = params.expectedErrors;
    REQUIRE(actualErrors.size() == expectedErrors.size());
    for (auto i : tlc::rv::iota(0ul, actualErrors.size())) {
        CAPTURE(i);
        REQUIRE(actualErrors[i].context() == expectedErrors[i].context);
        REQUIRE(actualErrors[i].reason() == expectedErrors[i].reason);
    }
}

inline auto ParseTestFixture::assertTranslationUnit(
    tlc::Str source, tlc::Str expected, SLoc location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }();
    REQUIRE(matchAstType<TranslationUnit>(result));
    REQUIRE(astCast<TranslationUnit>(result).sourcePath()
        == filepath);

    auto const actual = tlc::parse::ASTPrinter::operator()(std::move(result));
    REQUIRE(actual == expected);
}

inline auto ParseTestFixture::assertExpr(
    tlc::Str source, tlc::Str expected, SLoc const location
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

inline auto ParseTestFixture::assertString(
    tlc::Str source, tlc::Str expected, tlc::Vec<tlc::Str> expectedFragments,
    SLoc location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseExpr();
    REQUIRE(result.has_value());

    auto const actual = tlc::parse::ASTPrinter::operator()(*result);
    REQUIRE(actual == expected);

    REQUIRE(matchAstType<expr::String>(*result));
    for (auto&& [actualFragment,expectedFragment]
         : tlc::rv::zip(
             astCast<expr::String>(
                 *result).fragments(), expectedFragments
         )) {
        REQUIRE(actualFragment == expectedFragment);
    }
}

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
