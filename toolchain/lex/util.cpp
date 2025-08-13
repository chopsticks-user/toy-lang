export module lex:util;

import core;

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

    constexpr auto isCommentOuter(char const c) -> bool {
        return c == '\\';
    }

    constexpr auto isBinaryDigit(char const c) -> bool {
        return c == '0' || c == '1';
    }

    constexpr auto isOctalDigit(char const c) -> bool {
        return c >= '0' && c <= '7';
    }

    constexpr auto isHexadecimalDigit(char const c) -> bool {
        return isDigit(c) || (c >= 'a' && c <= 'f');
    }

    constexpr auto isStringTerminator(char const c) -> bool {
        return c == '"';
    }
}
