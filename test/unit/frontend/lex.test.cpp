#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

using tlc::lex::Lexer;
using tlc::Str;
using tlc::StrV;
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

    auto assertTokenAt(
        szt const i, EToken const type, StrV const str,
        szt const line, szt const column
    ) const -> void {
        CAPTURE(i);
        REQUIRE(i < m_tokens.size());
        REQUIRE(m_tokens[i].type() == type);
        REQUIRE(m_tokens[i].str() == str);
        REQUIRE(m_tokens[i].line() == line);
        REQUIRE(m_tokens[i].column() == column);
    }

    auto assertTokenCount(szt const count) const -> void {
        REQUIRE(count == m_tokens.size());
    }

private:
    Vec<Token> m_tokens;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Lex: Spaces correctly ignored", "[Lex]") {
    lex(R"(
foo Bar    Float
int
    module
    )");

    assertTokenCount(5);
    assertTokenAt(0, EToken::Identifier, "foo", 1, 0);
    assertTokenAt(1, EToken::UserDefinedType, "Bar", 1, 4);
    assertTokenAt(2, EToken::FundamentalType, "Float", 1, 11);
    assertTokenAt(3, EToken::Module, "module", 3, 4);
}
