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

    assertTokenCount(4);
    assertTokenAt(0, EToken::Identifier, "foo", 1, 0);
    assertTokenAt(1, EToken::UserDefinedType, "Bar", 1, 4);
    assertTokenAt(2, EToken::FundamentalType, "Float", 1, 11);
    // "int" is a reserved keyword
    assertTokenAt(3, EToken::Module, "module", 3, 4);
}

TEST_CASE_WITH_FIXTURE("Lex: Identifiers and keywords", "[Lex]") {
    SECTION("Fundamental types") {}

    SECTION("Reserved keywords") {}

    SECTION("Identifiers") {}

    SECTION("UserDefinedType") {}

    SECTION("Keywords") {
        SECTION("Visibility") {
            lex(R"(
module
import
export
internal
local
extern
            )");

            assertTokenCount(6);
            assertTokenAt(0, EToken::Module, "module", 1, 0);
            assertTokenAt(1, EToken::Import, "import", 2, 0);
            assertTokenAt(2, EToken::Export, "export", 3, 0);
            assertTokenAt(3, EToken::Internal, "internal", 4, 0);
            assertTokenAt(4, EToken::Local, "local", 5, 0);
            assertTokenAt(5, EToken::Extern, "extern", 6, 0);
        }

        SECTION("Definition") {
            lex(R"(
let
mut
fn
trait
type
enum
flag
            )");

            assertTokenCount(7);
            assertTokenAt(0, EToken::Let, "let", 1, 0);
            assertTokenAt(1, EToken::Mut, "mut", 2, 0);
            assertTokenAt(2, EToken::Fn, "fn", 3, 0);
            assertTokenAt(3, EToken::Trait, "trait", 4, 0);
            assertTokenAt(4, EToken::Type, "type", 5, 0);
            assertTokenAt(5, EToken::Enum, "enum", 6, 0);
            assertTokenAt(6, EToken::Flag, "flag", 7, 0);
        }

        SECTION("Control") {
            lex(R"(
for
return
match
            )");

            assertTokenCount(3);
            assertTokenAt(0, EToken::For, "for", 1, 0);
            assertTokenAt(1, EToken::Return, "return", 2, 0);
            assertTokenAt(2, EToken::Match, "match", 3, 0);
        }

        SECTION("Adverb") {
            lex(R"(
by
of
in
when
            )");

            assertTokenCount(4);
            assertTokenAt(0, EToken::By, "by", 1, 0);
            assertTokenAt(1, EToken::Of, "of", 2, 0);
            assertTokenAt(2, EToken::In, "in", 3, 0);
            assertTokenAt(3, EToken::When, "when", 4, 0);
        }

        SECTION("Boolean") {
            lex(R"(
true
false
            )");

            assertTokenCount(2);
            assertTokenAt(0, EToken::True, "true", 1, 0);
            assertTokenAt(1, EToken::False, "false", 2, 0);
        }

        SECTION("Object") {
            lex(R"(
self
pub
prv
impl
            )");

            assertTokenCount(4);
            assertTokenAt(0, EToken::Self, "self", 1, 0);
            assertTokenAt(1, EToken::Pub, "pub", 2, 0);
            assertTokenAt(2, EToken::Prv, "prv", 3, 0);
            assertTokenAt(3, EToken::Impl, "impl", 4, 0);
        }
    }
}

TEST_CASE_WITH_FIXTURE("Lex: Numbers", "[Lex]") {}

TEST_CASE_WITH_FIXTURE("Lex: Strings", "[Lex]") {}

TEST_CASE_WITH_FIXTURE("Lex: Symbols", "[Lex]") {}
