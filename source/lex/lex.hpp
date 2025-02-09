#ifndef TLC_LEX_HPP
#define TLC_LEX_HPP

#include "core/core.hpp"

#include "context.hpp"
#include "stream.hpp"

namespace tlc::lex {
}

// namespace tl::fe {
//   /**
//    * @brief
//    */
//   class Lexer {
//   public:
//     auto operator()(const fs::path &filepath) -> Tokens;
//
//     auto operator()(std::istringstream ss) -> Tokens;
//
//   private:
//     auto lex() -> Tokens;
//
//     auto advance() -> bool;
//
//     auto revertInline() -> bool;
//
//     auto peek() -> char;
//
//     auto peek2() -> std::string;
//
//     auto match(char expected) -> bool;
//
//     auto hasError() const -> bool {
//       return !m_collectedErrors.empty();
//     }
//
//     auto readNext() -> void {
//       m_fs->get(m_currentChar);
//     }
//
//     auto consume() -> void {
//       m_currentToken += m_currentChar;
//     }
//
//     auto skip(sz n = 1) -> void;
//
//     auto trySkip() -> bool;
//
//     auto addToken(EToken tokenType) -> void;
//
//     auto appendError(std::runtime_error error) -> void;
//
//     auto reset() -> void;
//
//   private:
//     static auto isDigit(char c) -> bool {
//       return !(c < '0' || c > '9');
//     }
//
//     static auto isUpperCaseLetter(char c) -> bool {
//       return !(c < 'A' || c > 'Z');
//     }
//
//     static auto isLowerCaseLetter(char c) -> bool {
//       return !(c < 'a' || c > 'z');
//     }
//
//     static auto isLetter(char c) -> bool {
//       return isLowerCaseLetter(c) || isUpperCaseLetter(c);
//     }
//
//     static auto isDigitOrLetter(char c) -> bool {
//       return isDigit(c) || isLetter(c);
//     }
//
//     static auto isStartOfInteger(char c) {
//       return isDigit(c);
//     }
//
//     static auto isUnnamedIdentifier(char c) {
//       return c == '_';
//     }
//
//     static auto isStartOfDecimalPart(char c) {
//       return c == '.';
//     }
//
//     static auto maybeOperatorCharacter(char c) -> bool {
//       return !isDigitOrLetter(c) && !isUnnamedIdentifier(c);
//     }
//
//     static auto isSpacingCharacter(char c) -> bool {
//       return c == ' ' || c == '\n' || c == '\t' || c == '\r';
//     }
//
//   private:
//     auto lexStringLiteral() -> bool;
//
//     auto lexNumericLiteral() -> bool;
//
//     auto lexIdentifier() -> bool;
//
//     auto lexOperator() -> bool;
//
//     auto handleWhiteSpace() -> bool;
//
//     auto handleNewLine() -> bool;
//
//     auto handleComment() -> bool;
//
//     auto handleEscapeSequence() -> bool;
//
//   private:
//     Ptr<std::istream> m_fs;
//     EToken m_lastTokenType = EToken::Empty;
//     c8 m_currentChar = '\0';
//     String m_currentToken;
//     Tokens m_collectedTokens;
//     sz m_currentLine = 0;
//     sz m_currentColumn = 0;
//     sz m_lastNonEmptyColumn = 0;
//     bool m_stringState = false;
//     Vec<std::runtime_error> m_collectedErrors;
//   };
// }

#endif // TLC_LEX_HPP
