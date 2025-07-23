#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ParseTestFixture {
protected:
    auto parse(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = tlc::parse::Parser::operator()(
            tlc::lex::Lexer::operator()(std::move(iss))
        );
    }

    static auto parseExpr(tlc::Str source)
        -> tlc::parse::Parser::ParseResult {
        std::istringstream iss;
        iss.str(std::move(source));
        return tlc::parse::Parser{
            tlc::lex::Lexer::operator()(std::move(iss))
        }.parseExpr();
    }

private:
    tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: Single-token literals", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Expr", "[Parse]") {
    auto expr = parseExpr("-x-3");
    REQUIRE(expr);
}
