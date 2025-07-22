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

private:
    tlc::syntax::Node m_ast;
    tlc::token::TokenizedBuffer m_tokens;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: Expressions", "[Parse]") {}
