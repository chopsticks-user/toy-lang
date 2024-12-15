#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Lexer.hpp"

using tl::fe::EToken;
using tl::fe::Tokens;
using tl::String;
using tl::u64;

class LexerTestFixture {
protected:
  auto lex(String source) -> void {
    std::istringstream iss;
    iss.str(std::move(source));
    m_tokens = tl::util::apply<tl::fe::Lexer>(std::move(iss));
  }

  auto nTokens() const -> u64 {
    return m_tokens.size();
  }

  auto contains(const EToken expected) const -> bool {
    return rng::find_if(
             m_tokens.begin(), m_tokens.end(),
             [&expected](const tl::fe::Token &token) {
               return token.type() == expected;
             }
           ) != m_tokens.end();
  }

  auto tokens() const -> const Tokens & {
    return m_tokens;
  }

private:
  Tokens m_tokens;
};

#define REQUIRE_TOKENS(...) \
  do { \
    tl::Vec<EToken> expected{__VA_ARGS__}; \
    auto actual = tokens(); \
    REQUIRE(actual.size() == expected.size()); \
    for (auto i: rv::iota(0ul, actual.size())) { \
        REQUIRE(expected[i] == actual[i].type()); \
    } \
  } \
  while(false)

#define TEST_CASE_WITH_FIXTURE(...) \
  TEST_CASE_METHOD(LexerTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Lexer: Module statement", "[Lexer]") {
  lex(R"(module foo;)");

  REQUIRE_TOKENS(
    EToken::Module,
    EToken::Identifier, // foo
    EToken::Semicolon
  );
}

TEST_CASE_WITH_FIXTURE("Lexer: Import statement", "[Lexer]") {
  lex(R"(
import foo;
import foo::bar;
  )");

  REQUIRE_TOKENS(
    // line 1
    EToken::Import,
    EToken::Identifier, // foo
    EToken::Semicolon,

    // line 2
    EToken::Import,
    EToken::Identifier, // foo
    EToken::Colon2,
    EToken::Identifier, // bar
    EToken::Semicolon
  );
}

TEST_CASE_WITH_FIXTURE("Lexer: Let statement", "[Lexer]") {
  lex(R"(
let x: Int, y: Type = foo, z = bar, t;
let mutable x: Int, mutable y: Type = foo, mutable z = bar, mutable t;
  )");

  REQUIRE_TOKENS(
    // line 1
    EToken::Let,
    EToken::Identifier, // x
    EToken::Colon,
    EToken::FundamentalType, // Int
    EToken::Comma,
    EToken::Identifier, // y
    EToken::Colon,
    EToken::UserDefinedType, // Type
    EToken::Equal,
    EToken::Identifier,
    EToken::Comma,
    EToken::Identifier, // z
    EToken::Equal,
    EToken::Identifier,
    EToken::Comma,
    EToken::Identifier, // t
    EToken::Semicolon,

    // line 2
    EToken::Let,
    EToken::Mutable,
    EToken::Identifier, // x
    EToken::Colon,
    EToken::FundamentalType, // Int
    EToken::Comma,
    EToken::Mutable,
    EToken::Identifier, // y
    EToken::Colon,
    EToken::UserDefinedType, // Type
    EToken::Equal,
    EToken::Identifier,
    EToken::Comma,
    EToken::Mutable,
    EToken::Identifier, // z
    EToken::Equal,
    EToken::Identifier,
    EToken::Comma,
    EToken::Mutable,
    EToken::Identifier, // t
    EToken::Semicolon,
  );
}

TEST_CASE_WITH_FIXTURE("Lexer: Tuple declaration", "[Lexer]") {
  lex(R"(
(x: Float, y: Float)
(_, product: Float, sum, foo: Type)
  )");

  REQUIRE_TOKENS(
    // line 1
    EToken::LeftParen,
    EToken::Identifier, // x
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::Comma,
    EToken::Identifier, // y
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::RightParen,

    // line 2
    EToken::LeftParen,
    EToken::AnnonymousIdentifier,
    EToken::Comma,
    EToken::Identifier, // product
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::Comma,
    EToken::Identifier, // sum
    EToken::Comma,
    EToken::Identifier, // foo
    EToken::Colon,
    EToken::UserDefinedType, // Type
    EToken::RightParen,
  );
}
