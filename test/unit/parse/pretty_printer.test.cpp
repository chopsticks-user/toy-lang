#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class PrettyPrinterTestFixture {
protected:
    auto parse(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = tlc::parse::Parse::operator()(
            tlc::lex::Lex::operator()(std::move(iss))
        );
    }

private:
    tlc::parse::Parse::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(PrettyPrinterTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("PrettyPrinter: ", "[Parse][PrettyPrinter]") {}
