#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

using tlc::Str;
using tlc::parse::Parser;
using tlc::lex::Lexer;

class ParseTestFixture {
protected:
    auto parse(Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = Parser::operator()(
            Lexer::operator()(std::move(iss))
        );
    }

private:
    tlc::syntax::Node m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: Expressions", "[Parse]") {}
