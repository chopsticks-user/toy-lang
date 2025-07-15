#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

using tlc::lex::Lexer;
using tlc::Str;
using tlc::Vec;
using tlc::szt;
using tlc::token::Token;
using tlc::token::EToken;

class LexTestFixture {
protected:
    auto lex(Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_tokens = tlc::apply<Lexer>(std::move(iss));
    }

private:
    Vec<Token> m_tokens;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexTestFixture, __VA_ARGS__)

// TEST_CASE_WITH_FIXTURE("Lex: Identifier", "[Lex]") {
//     lex(R"(
// test
//     )");
// }
