#ifndef TOYLANG_LEXER_TOKEN_HPP
#define TOYLANG_LEXER_TOKEN_HPP

#include "Core/Core.hpp"

namespace tl::lexer {
  enum class EToken {
    // Single
    LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace, RightBrace,
    Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus, FwdSlash, Percent,
    Exclaim, Equal, Greater, Less, SQuote, DQuote, QMark,

    // Double
    Colon2, Star2, Ampersand2, Bar2, Plus2, Minus2, FwdSlash2, Exclaim2, Equal2,
    Greater2, Less2, QMark2, ExclaimEqual, StarEqual, AmpersandEqual, BarEqual,
    FwdSlashEqual, PercentEqual, GreaterEqual, LessEqual, PlusEqual, MinusEqual, BarGreater,
    MinusGreater, LessMinus, Greater2Equal, Less2Equal,

    // Triple
    Dot3,

    // Multiple
    Identifier, Number,

    // Reserved keywords
    Fn, Main, Class, Super, Self, Public, Private, Protected, Return, For, While, If, Switch, Case,
    Default, Export, Import, Module, Internal, Local, Pure, Var, Const, Print, Extern, Abstract,
    Interface,

    // Misc
    Empty
  };

  class Token final {
  public:
    static auto isDigit(char c) noexcept -> bool {
      return c >= '0' && c <= '9';
    }

    static auto isIdentifierSymbol(char c) noexcept -> bool {
      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    static auto identifierType(const std::string &idStr) noexcept -> EToken;

    Token(EToken type, std::string str, sz line, sz column)
      : m_type(type), m_str(std::move(str)), m_line(line), m_column(column) {
    }

    auto type() const noexcept -> EToken {
      return m_type;
    }

    auto string() const noexcept -> const std::string & {
      return m_str;
    }

    auto line() const noexcept -> sz {
      return m_line;
    }

    auto column() const noexcept -> sz {
      return m_column;
    }

  private:
    EToken m_type;
    std::string m_str;
    sz m_line;
    sz m_column;
  };
}

#endif // TOYLANG_LEXER_TOKEN_HPP
