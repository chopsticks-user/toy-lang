#include "parse.test.hpp"

auto ParseTestFixture::assertExpr(
    AssertParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse.parseExpr();
        REQUIRE(result);
        return *result;
    });
}

auto ParseTestFixture::assertType(
    AssertParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse.parseType();
        REQUIRE(result);
        return *result;
    });
}

auto ParseTestFixture::assertDecl(AssertParams params, SLoc const location) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse.parseDecl();
        REQUIRE(result);
        return *result;
    });
}

auto ParseTestFixture::assertGenericDecl(AssertParams params, SLoc const location) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse.parseGenericParamsDecl();
        REQUIRE(result);
        return *result;
    });
}

auto ParseTestFixture::assertStmt(
    AssertParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse.parseStmt();
        REQUIRE(result);
        return *result;
    });
}

auto ParseTestFixture::assertTranslationUnit(
    AssertParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    parseAndAssert(std::move(params), [](tlc::parse::Parse parse) {
        auto result = parse();
        return result;
    });
}

auto ParseTestFixture::parseAndAssert(
    AssertParams params, Node (*fn)(tlc::parse::Parse)
) -> void {
    std::istringstream iss;
    iss.str(std::move(params.source));
    auto result = fn(tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    });

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

    auto const actualErrors =
        ErrCollector::instance().exportErrors();
    auto const expectedErrors = params.expectedErrors;
    REQUIRE(actualErrors.size() == expectedErrors.size());
    for (auto i : tlc::rv::iota(0ul, actualErrors.size())) {
        CAPTURE(i);
        REQUIRE(actualErrors[i].context() == expectedErrors[i].context);
        REQUIRE(actualErrors[i].reason() == expectedErrors[i].reason);
    }
}

// auto ParseTestFixture::assertTranslationUnit(
//     tlc::Str source, tlc::Str expected, SLoc location
// ) -> void {
//     INFO(std::format("{}:{}", location.file_name(), location.line()));
//     std::istringstream iss;
//     iss.str(std::move(source));
//
//     auto result = tlc::parse::Parse{
//         filepath, tlc::lex::Lex::operator()(std::move(iss))
//     }();
//     REQUIRE(matchAstType<TranslationUnit>(result));
//     REQUIRE(astCast<TranslationUnit>(result).sourcePath()
//         == filepath);
//
//     auto const actual = tlc::parse::ASTPrinter::operator()(std::move(result));
//     REQUIRE(actual == expected);
// }
