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
    REQUIRE(actual.size() >= expected.size()); \
    for (auto i: rv::iota(0ul, expected.size())) { \
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
let val = Type(-3.14159, true, []);
let (a: Float, b: Float) = (+2, -1);
let (_, product: Float, sum, val: Type) = compute(5, bar);
let arr1: Type[][];
let arr2 = [-10 .. 10 @ 2, ...[0 .. 5], 2, 3, 5 of -1, ...[]];
let arr3 = [];
let obj1 = {x: 1, y: [0 .. 5], z: {x: true, y: false}};
let obj2 = {};
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

    // line 3
    EToken::Let,
    EToken::Identifier, // val
    EToken::Equal,
    EToken::UserDefinedType, // Type
    EToken::LeftParen,
    EToken::Minus,
    EToken::FloatLiteral, // 3.14159
    EToken::Comma,
    EToken::True,
    EToken::Comma,
    EToken::LeftBracket,
    EToken::RightBracket,
    EToken::RightParen,
    EToken::Semicolon,

    // line 4
    EToken::Let,
    EToken::LeftParen,
    EToken::Identifier, // a
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::Comma,
    EToken::Identifier, // b
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::RightParen,
    EToken::Equal,
    EToken::LeftParen,
    EToken::Plus,
    EToken::IntegerLiteral, // 2
    EToken::Comma,
    EToken::Minus,
    EToken::IntegerLiteral, // 1
    EToken::RightParen,
    EToken::Semicolon,

    // line 5
    EToken::Let,
    EToken::LeftParen,
    EToken::AnnonymousIdentifier,
    EToken::Comma,
    EToken::Identifier, // product
    EToken::Colon,
    EToken::FundamentalType, // Float
    EToken::Comma,
    EToken::Identifier, // sum
    EToken::Comma,
    EToken::Identifier, // val
    EToken::Colon,
    EToken::UserDefinedType, // Type
    EToken::RightParen,
    EToken::Equal,
    EToken::Identifier, // compute
    EToken::LeftParen,
    EToken::IntegerLiteral,
    EToken::Comma,
    EToken::Identifier, // bar
    EToken::RightParen,
    EToken::Semicolon,

    // line 6
    EToken::Let,
    EToken::Identifier, // arr1
    EToken::Colon,
    EToken::UserDefinedType,
    EToken::LeftBracket,
    EToken::RightBracket,
    EToken::LeftBracket,
    EToken::RightBracket,
    EToken::Semicolon,

    // line 7
    EToken::Let,
    EToken::Identifier, // arr2
    EToken::Equal,
    EToken::LeftBracket,
    EToken::Minus,
    EToken::IntegerLiteral, // 10
    EToken::Dot2,
    EToken::IntegerLiteral, // 10
    EToken::At,
    EToken::IntegerLiteral, // 2
    EToken::Comma,
    EToken::Dot3,
    EToken::LeftBracket,
    EToken::IntegerLiteral, // 0
    EToken::Dot2,
    EToken::IntegerLiteral, // 5
    EToken::RightBracket,
    EToken::Comma,
    EToken::IntegerLiteral, // 2
    EToken::Comma,
    EToken::IntegerLiteral, // 3
    EToken::Comma,
    EToken::IntegerLiteral, // 5
    EToken::Of,
    EToken::Minus,
    EToken::IntegerLiteral, // 1
    EToken::Comma,
    EToken::Dot3,
    EToken::LeftBracket,
    EToken::RightBracket,
    EToken::RightBracket,
    EToken::Semicolon,

    // line 8
    EToken::Let,
    EToken::Identifier, // arr3
    EToken::Equal,
    EToken::LeftBracket,
    EToken::RightBracket,
    EToken::Semicolon,

    // line 9
    EToken::Let,
    EToken::Identifier, // rec1
    EToken::Equal,
    EToken::LeftBrace,
    EToken::Identifier, // x
    EToken::Colon,
    EToken::IntegerLiteral,
    EToken::Comma,
    EToken::Identifier, // y
    EToken::Colon,
    EToken::LeftBracket,
    EToken::IntegerLiteral,
    EToken::Dot2,
    EToken::IntegerLiteral,
    EToken::RightBracket,
    EToken::Comma,
    EToken::Identifier, // z
    EToken::Colon,
    EToken::LeftBrace,
    EToken::Identifier, // a
    EToken::Colon,
    EToken::True,
    EToken::Comma,
    EToken::Identifier, // b
    EToken::Colon,
    EToken::False,
    EToken::RightBrace,
    EToken::RightBrace,
    EToken::Semicolon,

    // line 10
    EToken::Let,
    EToken::Identifier, // rec2
    EToken::Equal,
    EToken::LeftBrace,
    EToken::RightBrace,
    EToken::Semicolon,
  );
}

TEST_CASE_WITH_FIXTURE("Lexer: Function prototype", "[Lexer]") {
  lex(R"(
(x: Float, y: Float)
(_, product: Float, sum, foo: Type)
  )");
}
