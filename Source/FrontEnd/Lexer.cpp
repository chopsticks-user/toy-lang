#include "Lexer.hpp"

namespace tl::fe {
  auto Lexer::advance() -> bool {
    if (m_finished) {
      return false;
    };

    readNext();
    m_lastNonEmptyColumn = m_currentColumn;
    if (!m_stringState && trySkip()) {
      return false;
    }

    ++m_currentColumn;
    return true;
  }

  auto Lexer::trySkip() -> bool {
    return handleWhiteSpace() || handleNewLine() || handleComment();
  }


  auto Lexer::match(char expected) -> bool {
    if (m_fs.eof() || m_fs.peek() != expected) {
      return false;
    }

    readNext();
    return true;
  }

  auto Lexer::reset(const std::string &filepath) -> void {
    m_fs = {};
    m_fs.open(filepath);
    if (!m_fs.is_open()) {
      throw std::runtime_error("Failed to open " + filepath);
    }

    m_lastTokenType = EToken::Empty;
    m_currentChar = '\0';
    m_currentToken = "";
    m_collectedTokens = {};
    m_currentLine = 0;
    m_currentColumn = 0;
    m_finished = false;
    m_collectedErrors = {};
  }

  auto Lexer::addToken(EToken tokenType) -> void {
    sz column = m_currentColumn, line = m_currentLine;
    if (m_currentColumn < m_currentToken.length() + 1) {
      column = m_lastNonEmptyColumn + 1;
      --line;
    }
    column -= m_currentToken.length() + 1;

    m_collectedTokens.emplace_back(
      tokenType, m_currentToken, m_currentLine, column
    );
    m_lastTokenType = m_collectedTokens.back().type();
  }

  auto Lexer::appendError(std::runtime_error error) -> void {
    m_collectedErrors.push_back(std::move(error));
  }

  auto Lexer::handleWhiteSpace() -> bool {
    if (m_currentChar == ' ') {
      ++m_currentColumn;
      return true;
    }

    if (m_currentChar == '\t') {
      m_currentColumn += 2;
      return true;
    }

    return false;
  }

  auto Lexer::handleNewLine() -> bool {
    if (m_currentChar != '\n' && m_currentChar != '\r') {
      return false;
    }

    m_currentColumn = 0;
    ++m_currentLine;
    return true;
  }

  auto Lexer::handleComment() -> bool {
    if (m_currentChar != '/') {
      return false;
    }

    if (match('/')) {
      std::string dummy;
      m_fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return true;
    }

    if (match('*')) {
      // todo:
    }

    return false;
  }

  auto Lexer::handleEndOfFile() const -> bool {
    if (m_currentChar == std::char_traits<char>::eof()) {
      return m_finished;
    }

    return m_finished;
  }

  auto Lexer::handleEscapeSequence() -> bool {
    return false;
  }

  auto Lexer::operator()(const std::string &filepath) -> Tokens {
    reset(filepath);

    while (!m_finished) {
      while (isSpacingCharacter(m_currentChar)) {
        advance();
      }
      m_currentToken = m_currentChar;

      if (lexStringLiteral()) {
        continue;
      }

      if (lexNumericLiteral()) {
        continue;
      }

      if (lexIdentifier()) {
        continue;
      }

      lexOperator(); // called last to match any token
    }

    if (hasError()) {
      throw std::runtime_error("Lexer error");
    }

    return m_collectedTokens;
  }

  auto Lexer::lexStringLiteral() -> bool {
    if (m_currentChar != '"') {
      return false;
    }

    m_stringState = true;

    while (advance()) {
      if (m_currentChar == '\\') {
        // todo:
      }

      consume();

      if (m_currentChar == '"') {
        break;
      }
    }

    addToken(EToken::StringLiteral);
    m_stringState = false;

    return true;
  }

  auto Lexer::lexNumericLiteral() -> bool {
    bool lexed = false;

    if (isStartOfInteger(m_currentChar)) {
      // accept both, then check if it is actually a number
      while (advance() && isDigitOrLetter(m_currentChar)) {
        consume();
      }

      addToken(EToken::IntegerLiteral);
      lexed = true;
    }

    if (isStartOfDecimalPart(m_currentChar)) {
      consume();

      // accept both, then check if it is actually a number
      while (advance() && isDigitOrLetter(m_currentChar)) {
        consume();
      }

      addToken(EToken::FloatLiteral);
      lexed = true;
    }

    if (lexed && m_lastTokenType == EToken::Invalid) {
      appendError(std::runtime_error("Invalid numeric literal"));
    }

    return lexed;
  }

  auto Lexer::lexIdentifier() -> bool {
    if (isUnnamedIdentifier(m_currentChar)) {
      if (isDigitOrLetter(peek())) {
        appendError(std::runtime_error("Identifiers cannot contain or be prefixed with '_'"));
        return false;
      }
      addToken(EToken::AnnonymousIdentifier);
      return true;
    }

    if (!isLetter(m_currentChar)) {
      return false;
    }

    bool containsUnderscore = false;
    while (advance() && (isDigitOrLetter(m_currentChar) || isUnnamedIdentifier(m_currentChar))) {
      // one exception thrown per token
      if (!containsUnderscore && isUnnamedIdentifier(m_currentChar)) {
        containsUnderscore = true;
        appendError(std::runtime_error("Identifiers cannot contain or be prefixed with '_'"));
      }

      consume();
    }

    addToken(EToken::Identifier);
    if (m_lastTokenType == EToken::Reserved) {
      appendError(std::runtime_error("Reserved keyword"));
    }

    return true;
  }

  auto Lexer::lexOperator() -> bool {
    while (advance() && maybeOperatorCharacter(m_currentChar)) {
      consume();
    }

    addToken(EToken::MaybeOperator);
    if (m_lastTokenType == EToken::Invalid) {
      appendError(std::runtime_error("Unrecognizable token"));
    }

    return true;
  }
}
