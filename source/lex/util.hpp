#ifndef TLC_LEX_UTIL_HPP
#define TLC_LEX_UTIL_HPP

#include "core/core.hpp"

namespace tlc::lex {
  constexpr auto isDigit(char const c) -> bool {
    return !(c < '0' || c > '9');
  }

  constexpr auto isUpperCaseLetter(char const c) -> bool {
    return !(c < 'A' || c > 'Z');
  }

  constexpr auto isLowerCaseLetter(char const c) -> bool {
    return !(c < 'a' || c > 'z');
  }

  constexpr auto isLetter(char const c) -> bool {
    return isLowerCaseLetter(c) || isUpperCaseLetter(c);
  }

  constexpr auto isDigitOrLetter(char const c) -> bool {
    return isDigit(c) || isLetter(c);
  }

  constexpr auto isStartOfInteger(char const c) -> bool {
    return isDigit(c);
  }

  constexpr auto isUnnamedIdentifier(char const c) -> bool {
    return c == '_';
  }

  constexpr auto isStartOfDecimalPart(char const c) -> bool {
    return c == '.';
  }

  constexpr auto maybeOperatorCharacter(char const c) -> bool {
    return !isDigitOrLetter(c) && !isUnnamedIdentifier(c);
  }

  constexpr auto isSpacingCharacter(char const c) -> bool {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
  }
}

#endif // TLC_LEX_UTIL_HPP
