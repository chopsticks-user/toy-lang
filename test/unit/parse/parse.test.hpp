#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>

import core;
import lex;
import parse;
import syntax;

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

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
    static constexpr auto assertExpr(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse.parseExpr();
            REQUIRE(result);
            return *result;
        });
    }

    static constexpr auto assertType(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse.parseType();
            REQUIRE(result);
            return *result;
        });
    }

    static constexpr auto assertDecl(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse.parseDecl();
            REQUIRE(result);
            return *result;
        });
    }

    static constexpr auto assertGenericDecl(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse.parseGenericParamsDecl();
            REQUIRE(result);
            return *result;
        });
    }

    static constexpr auto assertStmt(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse.parseStmt();
            REQUIRE(result);
            return *result;
        });
    }

    static constexpr auto assertTranslationUnit(
        AssertParams params, SLoc const location = SLoc::current()
    ) -> void {
        INFO(std::format("{}:{}", location.file_name(), location.line()));
        parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
            auto result = parse();
            return result;
        });
    }

private:
    static constexpr auto parseAndAssert(
        AssertParams params, Node (*fn)(tlc::parse::Parse)
    ) -> void {
        std::istringstream iss;
        iss.str(std::move(params.source));
        auto result = fn(tlc::parse::Parse{
            filepath, tlc::lex::Lex::operator()(std::move(iss))
        });
        // place here to make sure errors are flushed before exiting
        auto const actualErrors =
            ErrCollector::instance().exportErrors();

        params.expectedAstPrint.transform(
            [&](auto&& expectedAstPrint) {
                auto const actualAstPrint =
                    tlc::parse::ASTPrinter::operator()(result);
                REQUIRE(actualAstPrint == expectedAstPrint);
                return "";
            }
        );

        params.expectedPrettyPrint.transform(
            [&]([[maybe_unused]] auto&& expectedPrettyPrint) {
                auto const actualPrettyPrint =
                    tlc::parse::PrettyPrint::operator()(result);
                REQUIRE(actualPrettyPrint == expectedPrettyPrint);
                return "";
            }
        );

        auto const expectedErrors = params.expectedErrors;
        REQUIRE(actualErrors.size() == expectedErrors.size());
        for (auto i : tlc::rv::iota(0ul, actualErrors.size())) {
            CAPTURE(i);
            REQUIRE(actualErrors[i].context() == expectedErrors[i].context);
            REQUIRE(actualErrors[i].reason() == expectedErrors[i].reason);
        }
    }
};

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
