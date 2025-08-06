#include "parse.test.hpp"

auto ParseTestFixture::assertExprWithParams(
    AssertParams params, SLoc location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    std::istringstream iss;
    iss.str(std::move(params.source));

    auto result = tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }.parseExpr();
    REQUIRE(result);

    params.expectedAstPrint.transform(
        [&](auto&& expectedAstPrint) {
            auto const actualAstPrint =
                tlc::parse::ASTPrinter::operator()(*result);
            REQUIRE(actualAstPrint == expectedAstPrint);
            return "";
        }
    );

    params.expectedPrettyPrint.transform(
        [&]([[maybe_unused]] auto&& expectedPrettyPrint) {
            auto const actualPrettyPrint =
                tlc::parse::PrettyPrint::operator()(std::move(*result));
            REQUIRE(actualPrettyPrint == expectedPrettyPrint);
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

auto ParseTestFixture::assertTranslationUnit(
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

auto ParseTestFixture::assertExpr(
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

auto ParseTestFixture::assertString(
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
