#ifndef TOYLANG_FRONTEND_TOKEN_HPP
#define TOYLANG_FRONTEND_TOKEN_HPP

#include "Core/Core.hpp"

namespace tl::fe {
  enum class EToken {
    // Single
    LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace, RightBrace,
    Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus, FwdSlash, Percent,
    Exclaim, Equal, Greater, Less, SQuote, DQuote, QMark, Hat, Tilde, Dollar,

    AnnonymousIdentifier,

    // Double
    Colon2, Star2, Ampersand2, Bar2, Plus2, Minus2, Exclaim2, Equal2,
    Greater2, Less2, QMark2, ExclaimEqual, StarEqual, AmpersandEqual, BarEqual,
    FwdSlashEqual, PercentEqual, GreaterEqual, LessEqual, PlusEqual, MinusEqual, BarGreater,
    MinusGreater, LessMinus, Dot2, HatEqual,

    // Triple
    Dot3, Greater2Equal, Less2Equal,

    // Multiple
    Identifier, IntegerLiteral, FloatLiteral, StringLiteral, FundamentalType, UserDefinedType,

    // Non-type reserved keywords
    Fn, Main, Class, Super, Self, Public, Private, Protected, Return, For, While, If, Switch, Case,
    Default, Export, Import, Module, Internal, Local, Pure, Var, Const, Print, Extern, Abstract,
    Interface, By,

    // Misc
    Empty, MaybeOperator, Reserved, Invalid,
  };

  enum class EKeyword {
  };

  class Token final {
  public:
    Token(EToken type, std::string str, sz line, sz column);

    static auto isValidOperator(const std::string &symbol) -> bool;

    auto type() const noexcept -> EToken {
      return m_type;
    }

    auto string() const noexcept -> const std::string & {
      return m_str;
    }

    auto line() const noexcept -> sz {
      return m_line + 1;
    }

    auto column() const noexcept -> sz {
      return m_column + 1;
    }

  private:
    EToken m_type;
    std::string m_str;
    sz m_line;
    sz m_column;
  };

  using Tokens = std::vector<Token>;
}

#endif // TOYLANG_FRONTEND_TOKEN_HPP
