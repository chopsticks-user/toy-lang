#include "Lexer.hpp"

namespace tl::fe {
  auto Lexer::advance() -> bool {
    if (m_fs->eof()) {
      return false;
    }

    readNext();
    m_lastNonEmptyColumn = m_currentColumn;
    if (!m_stringState && trySkip()) {
      return false;
    }

    ++m_currentColumn;
    return true;
  }

  auto Lexer::revertInline() -> bool {
    if (m_currentColumn == 0) {
      return false;
    }

    m_fs->unget();
    --m_currentColumn;
    return true;
  }


  auto Lexer::trySkip() -> bool {
    return handleWhiteSpace() || handleNewLine() || handleComment();
  }

  auto Lexer::skip(sz n) -> void {
    for (sz i = 0; i < n; ++i) {
      consume();
      advance();
    }
  }

  auto Lexer::match(char expected) -> bool {
    if (m_fs->eof() || m_fs->peek() != expected) {
      return false;
    }

    readNext();
    return true;
  }

  auto Lexer::peek() -> char {
    return static_cast<char>(m_fs->peek());
  }

  auto Lexer::peek2() -> std::string {
    std::string str;
    str += peek();
    str += static_cast<char>(m_fs->get());
    m_fs->unget();
    return str;
  }

  auto Lexer::reset() -> void {
    m_lastTokenType = EToken::Empty;
    m_currentChar = '\0';
    m_currentToken = "";
    m_collectedTokens = {};
    m_currentLine = 0;
    m_currentColumn = 0;
    m_collectedErrors = {};
  }

  auto Lexer::addToken(EToken tokenType) -> void {
    sz column = m_currentColumn, line = m_currentLine;
    if (m_currentColumn < m_currentToken.length() + 1) {
      column = m_lastNonEmptyColumn + 1;
      --line;
    }
    column -= m_currentToken.length() + 1;

    if (tokenType == EToken::StringLiteral) {
      column++;
    }

    m_collectedTokens.emplace_back(
      tokenType, m_currentToken, line, column
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
      m_fs->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return true;
    }

    if (match('*')) {
      // todo:
    }

    return false;
  }

  auto Lexer::handleEscapeSequence() -> bool {
    return false;
  }

  auto Lexer::operator()(const fs::path &filepath) -> Tokens {
    reset();
    auto fs = std::make_unique<std::ifstream>();
    fs->open(filepath);
    if (!fs->is_open()) {
      throw std::runtime_error("Failed to open " + filepath.string());
    }
    m_fs = std::move(fs);
    return lex();
  }

  auto Lexer::operator()(std::istringstream ss) -> Tokens {
    reset();
    m_fs = std::make_unique<std::istringstream>(std::move(ss));
    return lex();
  }

  auto Lexer::lex() -> Tokens {
    advance();
    while (true) {
      while (isSpacingCharacter(m_currentChar) && !m_fs->eof()) {
        advance();
      }
      if (m_fs->eof()) {
        break;
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

    // in case the lexer will be used for more than once
    m_fs.reset();
    return m_collectedTokens;
  }

  auto Lexer::lexStringLiteral() -> bool {
    if (m_currentChar != '"') {
      return false;
    }

    m_stringState = true;
    bool closed = false;

    while (advance()) {
      if (m_currentChar == '\\') {
        // todo:
      }

      consume();

      if (m_currentChar == '"') {
        closed = true;
        advance();
        consume();
        break;
      }
    }

    if (!closed) {
      // todo: handle unclosed string literal
      appendError(std::runtime_error("Un-closed string literal"));
    }

    addToken(EToken::StringLiteral);
    m_stringState = false;

    return true;
  }

  auto Lexer::lexNumericLiteral() -> bool {
    bool lexed = false;

    if (isStartOfInteger(m_currentChar)) {
      auto tokenType = EToken::IntegerLiteral;

      // accept both, then check if it is actually a number
      while (advance() && isDigit(m_currentChar)) {
        consume();
      }

      if (isStartOfDecimalPart(m_currentChar) && isDigit(peek())) {
        consume();

        // accept both, then check if it is actually a number
        while (advance() && isDigit(m_currentChar)) {
          consume();
        }

        tokenType = EToken::FloatLiteral;
      }

      addToken(tokenType);
      lexed = true;
    } else if (isStartOfDecimalPart(m_currentChar) && isDigit(peek())) {
      consume();

      // accept both, then check if it is actually a number
      while (advance() && isDigit(m_currentChar)) {
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

      advance();
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
    advance();

    // Maximum number of chracters is 3
    if (Token::isValidOperator(m_currentToken + m_currentChar + peek())) {
      skip(2);
      addToken(EToken::MaybeOperator); // valid 3-char operator
      return true;
    }

    if (Token::isValidOperator(m_currentToken + m_currentChar)) {
      skip();
      addToken(EToken::MaybeOperator); // valid 2-char operator
      return true;
    }

    addToken(EToken::MaybeOperator);
    if (m_lastTokenType == EToken::Invalid) {
      appendError(std::runtime_error("Unrecognizable token 0"));
      return false;
    }

    return true;
  }
}
