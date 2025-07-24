#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ASTPrinterTestFixture {
protected:
    auto parse(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = tlc::parse::Parser::operator()(
            tlc::lex::Lexer::operator()(std::move(iss))
        );
    }

private:
    tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ASTPrinterTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("ASTPrinter: ", "[Parse][ASTPrinter]") {}
