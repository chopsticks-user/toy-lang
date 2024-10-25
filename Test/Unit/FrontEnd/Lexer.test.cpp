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


TEST_CASE_METHOD(LexerTestFixture, "Lexer: Module declaration", "[Lexer]") {
  lex(R"(module foo;\n)");

  REQUIRE_TOKENS(
    EToken::Module, EToken::Identifier, EToken::Semicolon
  );
}

TEST_CASE_METHOD(LexerTestFixture, "Lexer: Import declaration", "[Lexer]") {
  lex(R"(
    import foo;
    import bar;
    import foo::bar;
  )");

  REQUIRE_TOKENS(
    EToken::Import, EToken::Identifier, EToken::Semicolon, // line 1
    EToken::Import, EToken::Identifier, EToken::Semicolon, // line 2
    EToken::Import, EToken::Identifier, EToken::Colon2, // line 3
    EToken::Identifier, EToken::Semicolon
  );
}
