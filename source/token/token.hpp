#ifndef TLC_TOKEN_HPP
#define TLC_TOKEN_HPP

#include "core/core.hpp"

namespace tlc::token {
  enum class EToken {
    // Single
    LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace, RightBrace, Hash,
    Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus, FwdSlash, Percent,
    Exclaim, Equal, Greater, Less, SQuote, DQuote, QMark, Hat, Tilde, Dollar, At,
    AnonymousIdentifier,

    // Double
    Colon2, Star2, Ampersand2, Bar2, Plus2, Minus2, Exclaim2, Equal2,
    Greater2, Less2, QMark2, ExclaimEqual, StarEqual, AmpersandEqual, BarEqual,
    FwdSlashEqual, PercentEqual, GreaterEqual, LessEqual, PlusEqual, MinusEqual, BarGreater,
    MinusGreater, LessMinus, Dot2, HatEqual, EqualGreater,

    // Triple
    Dot3, Greater2Equal, Less2Equal, ColonEqualGreater, ColonTildeGreater, Star2Equal,

    // Multiple
    Identifier, IntegerLiteral, FloatLiteral, StringLiteral, FundamentalType, UserDefinedType,

    // Keywords
    Module, Import, Export, Internal, Local, Extern, Static,
    Let, Mut, Fn, Trait, Type, Global, Enum, Flag,
    For, Return, Match,
    By, Of, In, When,
    True, False,
    Self, Pub, Prv, Impl,

    // Misc
    Empty, MaybeOperator, Reserved, Invalid,
  };

  enum class EKeyword {
  };

  class Token final {
  public:
    Token(EToken type, Str str, sz line, sz column);

    static auto isValidOperator(StrV symbol) -> bool;

    auto type() const noexcept -> EToken {
      return m_type;
    }

    auto string() const noexcept -> StrV {
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
    Str m_str;
    sz m_line;
    sz m_column;
  };
}

#endif // TLC_TOKEN_HPP
