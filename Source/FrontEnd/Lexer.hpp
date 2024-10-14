#ifndef TOYLANG_FRONTEND_LEXER_HPP
#define TOYLANG_FRONTEND_LEXER_HPP

#include "Token.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  using Tokens = std::vector<Token>;

  /**
   * @brief
   */
  class Lexer {
  public:
    auto operator()(const std::string &filepath) -> Tokens;

  private:
    auto advance() -> bool;

    auto peek() -> char {
      return static_cast<char>(m_fs.peek());
    }

    auto match(char expected) -> bool;

    auto hasError() const -> bool {
      return !m_collectedErrors.empty();
    }

    auto readNext() -> void {
      m_fs.get(m_currentChar);
    }

    auto consume() -> void {
      m_currentToken += m_currentChar;
    }

    auto trySkip() -> bool;

    auto addToken(EToken tokenType) -> void;

    auto appendError(std::runtime_error error) -> void;

    auto reset(const std::string &filepath) -> void;

  private:
    static auto isDigit(char c) -> bool {
      return !(c < '0' || c > '9');
    }

    static auto isUpperCaseLetter(char c) -> bool {
      return !(c < 'A' || c > 'Z');
    }

    static auto isLowerCaseLetter(char c) -> bool {
      return !(c < 'A' || c > 'Z');
    }

    static auto isLetter(char c) -> bool {
      return isLowerCaseLetter(c) || isUpperCaseLetter(c);
    }

    static auto isDigitOrLetter(char c) -> bool {
      return isDigit(c) || isLetter(c);
    }

    static auto isStartOfInteger(char c) {
      return isDigit(c);
    }

    static auto isUnnamedIdentifier(char c) {
      return c == '_';
    }

    static auto isStartOfDecimalPart(char c) {
      return c == '.';
    }

  private:
    auto lexStringLiteral() -> bool;

    auto lexNumericLiteral() -> bool;

    auto lexIdentifier() -> bool;

    auto lexOperator() -> bool;

    auto handleEndOfFile() const -> bool;

    auto handleWhiteSpace() -> bool;

    auto handleNewLine() -> bool;

    auto handleComment() -> bool;

    auto handleEscapeSequence() -> bool;

  private:
    std::fstream m_fs;
    EToken m_lastTokenType = EToken::Empty;
    c8 m_currentChar = '\0';
    std::string m_currentToken;
    Tokens m_collectedTokens;
    sz m_currentLine = 0;
    sz m_currentColumn = 0;
    bool m_stringState = false;
    bool m_finished = false;
    std::vector<std::runtime_error> m_collectedErrors;
  };
}

#endif // TOYLANG_FRONTEND_LEXER_HPP
