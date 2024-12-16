#include <catch2/catch_test_macros.hpp>

#include "FrontEnd/Lexer.hpp"

using tl::fe::EToken;
using tl::fe::Tokens;
using tl::fe::Token;
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
             [&expected](const Token &token) {
               return token.type() == expected;
             }
           ) != m_tokens.end();
  }

  static auto tokensEqual(const Token &l, const Token &r) -> bool {
    return l.line() == r.line() &&
           l.column() == r.column() &&
           l.string() == r.string() &&
           l.type() == r.type();
  }

  auto tokens() const -> const Tokens & {
    return m_tokens;
  }

private:
  Tokens m_tokens;
};

#define REQUIRE_TOKENS(...) \
  do { \
    tl::Vec<Token> expected{__VA_ARGS__}; \
    auto actual = tokens(); \
    REQUIRE(actual.size() == expected.size()); \
    for (auto i: rv::iota(0ul, expected.size())) { \
        CAPTURE(i); \
        CAPTURE(expected[i].line(), actual[i].line()); \
        CAPTURE(expected[i].column(), actual[i].column()); \
        CAPTURE(expected[i].string(), actual[i].string()); \
        CAPTURE(expected[i].type(), actual[i].type()); \
        REQUIRE(tokensEqual(expected[i], actual[i])); \
    } \
  } \
  while(false)

#define TEST_CASE_WITH_FIXTURE(...) \
  TEST_CASE_METHOD(LexerTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Lexer: Module statement", "[Lexer]") {
  lex(R"(
module foo;
)");

  REQUIRE_TOKENS(
    {EToken::Module, "module", 1, 0},
    {EToken::Identifier, "foo", 1, 7},
    {EToken::Semicolon, ";", 1, 10}
  );
}

TEST_CASE_WITH_FIXTURE("Lexer: Import statement", "[Lexer]") {
  lex(R"(
import foo;
import foo::bar;
  )");

  REQUIRE_TOKENS(
    {EToken::Import, "import", 1, 0},
    {EToken::Identifier, "foo", 1, 7},
    {EToken::Semicolon, ";", 1, 10},

    {EToken::Import, "import", 2, 0},
    {EToken::Identifier, "foo", 2, 7},
    {EToken::Colon2, "::", 2, 10},
    {EToken::Identifier, "bar", 2, 12},
    {EToken::Semicolon, ";", 2, 15}
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
    {EToken::Let, "let", 1, 0},
    {EToken::Identifier, "x", 1, 4},
    {EToken::Colon, ":", 1, 5},
    {EToken::FundamentalType, "Int", 1, 7},
    {EToken::Comma, ",", 1, 10},
    {EToken::Identifier, "y", 1, 12},
    {EToken::Colon, ":", 1, 13},
    {EToken::UserDefinedType, "Type", 1, 15},
    {EToken::Equal, "=", 1, 20},
    {EToken::Identifier, "foo", 1, 22},
    {EToken::Comma, ",", 1, 25},
    {EToken::Identifier, "z", 1, 27},
    {EToken::Equal, "=", 1, 29},
    {EToken::Identifier, "bar", 1, 31},
    {EToken::Comma, ",", 1, 34},
    {EToken::Identifier, "t", 1, 36},
    {EToken::Semicolon, ";", 1, 37},

    {EToken::Let, "let", 2, 0},
    {EToken::Mutable, "mutable", 2, 4},
    {EToken::Identifier, "x", 2, 12},
    {EToken::Colon, ":", 2, 13},
    {EToken::FundamentalType, "Int", 2, 15},
    {EToken::Comma, ",", 2, 18},
    {EToken::Mutable, "mutable", 2, 20},
    {EToken::Identifier, "y", 2, 28},
    {EToken::Colon, ":", 2, 29},
    {EToken::UserDefinedType, "Type", 2, 31},
    {EToken::Equal, "=", 2, 36},
    {EToken::Identifier, "foo", 2, 38},
    {EToken::Comma, ",", 2, 41},
    {EToken::Mutable, "mutable", 2, 43},
    {EToken::Identifier, "z", 2, 51},
    {EToken::Equal, "=", 2, 53},
    {EToken::Identifier, "bar", 2, 55},
    {EToken::Comma, ",", 2, 58},
    {EToken::Mutable, "mutable", 2, 60},
    {EToken::Identifier, "t", 2, 68},
    {EToken::Semicolon, ";", 2, 69},

    {EToken::Let, "let", 3, 0},
    {EToken::Identifier, "val", 3, 4}, // val
    {EToken::Equal, "=", 3, 8},
    {EToken::UserDefinedType, "Type", 3, 10},
    {EToken::LeftParen, "(", 3, 14},
    {EToken::Minus, "-", 3, 15},
    {EToken::FloatLiteral, "3.14159", 3, 16},
    {EToken::Comma, ",", 3, 23},
    {EToken::True, "true", 3, 25},
    {EToken::Comma, ",", 3, 29},
    {EToken::LeftBracket, "[", 3, 31},
    {EToken::RightBracket, "]", 3, 32},
    {EToken::RightParen, ")", 3, 33},
    {EToken::Semicolon, ";", 3, 34},

    {EToken::Let, "let", 4, 0},
    {EToken::LeftParen, "(", 4, 4},
    {EToken::Identifier, "a", 4, 5},
    {EToken::Colon, ":", 4, 6},
    {EToken::FundamentalType, "Float", 4, 8},
    {EToken::Comma, ",", 4, 13},
    {EToken::Identifier, "b", 4, 15},
    {EToken::Colon, ":", 4, 16},
    {EToken::FundamentalType, "Float", 4, 18},
    {EToken::RightParen, ")", 4, 23},
    {EToken::Equal, "=", 4, 25},
    {EToken::LeftParen, "(", 4, 27},
    {EToken::Plus, "+", 4, 28},
    {EToken::IntegerLiteral, "2", 4, 29},
    {EToken::Comma, ",", 4, 30},
    {EToken::Minus, "-", 4, 32},
    {EToken::IntegerLiteral, "1", 4, 33},
    {EToken::RightParen, ")", 4, 34},
    {EToken::Semicolon, ";", 4, 35},

    {EToken::Let, "let", 5, 0},
    {EToken::LeftParen, "(", 5, 4},
    {EToken::AnnonymousIdentifier, "_", 5, 5},
    {EToken::Comma, ",", 5, 6},
    {EToken::Identifier, "product", 5, 8},
    {EToken::Colon, ":", 5, 15},
    {EToken::FundamentalType, "Float", 5, 17},
    {EToken::Comma, ",", 5, 22},
    {EToken::Identifier, "sum", 5, 24},
    {EToken::Comma, ",", 5, 27},
    {EToken::Identifier, "val", 5, 29},
    {EToken::Colon, ":", 5, 32},
    {EToken::UserDefinedType, "Type", 5, 34},
    {EToken::RightParen, ")", 5, 38},
    {EToken::Equal, "=", 5, 40},
    {EToken::Identifier, "compute", 5, 42},
    {EToken::LeftParen, "(", 5, 49},
    {EToken::IntegerLiteral, "5", 5, 50},
    {EToken::Comma, ",", 5, 51},
    {EToken::Identifier, "bar", 5, 53},
    {EToken::RightParen, ")", 5, 56},
    {EToken::Semicolon, ";", 5, 57},

    {EToken::Let, "let", 6, 0},
    {EToken::Identifier, "arr1", 6, 4},
    {EToken::Colon, ":", 6, 8},
    {EToken::UserDefinedType, "Type", 6, 10},
    {EToken::LeftBracket, "[", 6, 14},
    {EToken::RightBracket, "]", 6, 15},
    {EToken::LeftBracket, "[", 6, 16},
    {EToken::RightBracket, "]", 6, 17},
    {EToken::Semicolon, ";", 6, 18},

    {EToken::Let, "let", 7, 0},
    {EToken::Identifier, "arr2", 7, 4},
    {EToken::Equal, "=", 7, 9},
    {EToken::LeftBracket, "[", 7, 11},
    {EToken::Minus, "-", 7, 12},
    {EToken::IntegerLiteral, "10", 7, 13},
    {EToken::Dot2, "..", 7, 16},
    {EToken::IntegerLiteral, "10", 7, 19},
    {EToken::At, "@", 7, 22},
    {EToken::IntegerLiteral, "2", 7, 24},
    {EToken::Comma, ",", 7, 25},
    {EToken::Dot3, "...", 7, 27},
    {EToken::LeftBracket, "[", 7, 30},
    {EToken::IntegerLiteral, "0", 7, 31},
    {EToken::Dot2, "..", 7, 33},
    {EToken::IntegerLiteral, "5", 7, 36},
    {EToken::RightBracket, "]", 7, 37},
    {EToken::Comma, ",", 7, 38},
    {EToken::IntegerLiteral, "2", 7, 40},
    {EToken::Comma, ",", 7, 41},
    {EToken::IntegerLiteral, "3", 7, 43},
    {EToken::Comma, ",", 7, 44},
    {EToken::IntegerLiteral, "5", 7, 46},
    {EToken::Of, "of", 7, 48},
    {EToken::Minus, "-", 7, 51},
    {EToken::IntegerLiteral, "1", 7, 52},
    {EToken::Comma, ",", 7, 53},
    {EToken::Dot3, "...", 7, 55},
    {EToken::LeftBracket, "[", 7, 58},
    {EToken::RightBracket, "]", 7, 59},
    {EToken::RightBracket, "]", 7, 60},
    {EToken::Semicolon, ";", 7, 61},

    {EToken::Let, "let", 8, 0},
    {EToken::Identifier, "arr3", 8, 4},
    {EToken::Equal, "=", 8, 9},
    {EToken::LeftBracket, "[", 8, 11},
    {EToken::RightBracket, "]", 8, 12},
    {EToken::Semicolon, ";", 8, 13},

    {EToken::Let, "let", 9, 0},
    {EToken::Identifier, "obj1", 9, 4},
    {EToken::Equal, "=", 9, 9},
    {EToken::LeftBrace, "{", 9, 11},
    {EToken::Identifier, "x", 9, 12},
    {EToken::Colon, ":", 9, 13},
    {EToken::IntegerLiteral, "1", 9, 15},
    {EToken::Comma, ",", 9, 16},
    {EToken::Identifier, "y", 9, 18},
    {EToken::Colon, ":", 9, 19},
    {EToken::LeftBracket, "[", 9, 21},
    {EToken::IntegerLiteral, "0", 9, 22},
    {EToken::Dot2, "..", 9, 24},
    {EToken::IntegerLiteral, "5", 9, 27},
    {EToken::RightBracket, "]", 9, 28},
    {EToken::Comma, ",", 9, 29},
    {EToken::Identifier, "z", 9, 31},
    {EToken::Colon, ":", 9, 32},
    {EToken::LeftBrace, "{", 9, 34},
    {EToken::Identifier, "x", 9, 35},
    {EToken::Colon, ":", 9, 36},
    {EToken::True, "true", 9, 38},
    {EToken::Comma, ",", 9, 42},
    {EToken::Identifier, "y", 9, 44},
    {EToken::Colon, ":", 9, 45},
    {EToken::False, "false", 9, 47},
    {EToken::RightBrace, "}", 9, 52},
    {EToken::RightBrace, "}", 9, 53},
    {EToken::Semicolon, ";", 9, 54},

    {EToken::Let, "let", 10, 0},
    {EToken::Identifier, "obj2", 10, 4},
    {EToken::Equal, "=", 10, 9},
    {EToken::LeftBrace, "{", 10, 11},
    {EToken::RightBrace, "}", 10, 12},
    {EToken::Semicolon, ";", 10, 13},
  );
}

// TEST_CASE_WITH_FIXTURE("Lexer: Function prototype", "[Lexer]") {
//   lex(R"(
// (x: Float, y: Float)
// (_, product: Float, sum, foo: Type)
//   )");
// }
