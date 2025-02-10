#ifndef TLC_LEX_HPP
#define TLC_LEX_HPP

#include "core/core.hpp"
#include "token/token.hpp"

#include "context.hpp"
#include "stream.hpp"
#include "util.hpp"

namespace tlc::lex {
  class Lexer {
  public:
    auto operator()(const fs::path &filepath) -> Vec<token::Token>;

    auto operator()(std::istringstream ss) -> Vec<token::Token>;

  private:
    auto lex() -> Vec<token::Token>;

    auto advance() -> bool;

    auto revertInline() -> bool;

    auto peek() -> char;

    auto peek2() -> std::string;

    auto match(char expected) -> bool;

    auto hasError() const -> bool {
      return !m_collectedErrors.empty();
    }

    auto readNext() -> void {
      m_fs->get(m_currentChar);
    }

    auto consume() -> void {
      m_currentToken += m_currentChar;
    }

    auto skip(sz n = 1) -> void;

    auto trySkip() -> bool;

    auto addToken(token::EToken tokenType) -> void;

    auto appendError(std::runtime_error error) -> void;

    auto reset() -> void;

  protected:
    auto lexStringLiteral() -> bool;

    auto lexNumericLiteral() -> bool;

    auto lexIdentifier() -> bool;

    auto lexOperator() -> bool;

    auto handleWhiteSpace() -> bool;

    auto handleNewLine() -> bool;

    auto handleComment() -> bool;

    auto handleEscapeSequence() -> bool;

  private:
    Ptr<std::istream> m_fs;
    token::EToken m_lastTokenType = token::EToken::Empty;
    c8 m_currentChar = '\0';
    Str m_currentToken;
    Vec<token::Token> m_collectedTokens;
    sz m_currentLine = 0;
    sz m_currentColumn = 0;
    sz m_lastNonEmptyColumn = 0;
    bool m_stringState = false;
    Vec<std::runtime_error> m_collectedErrors;
  };
}


#endif // TLC_LEX_HPP
