#ifndef TOYLANG_FRONTEND_TOKEN_HPP
#define TOYLANG_FRONTEND_TOKEN_HPP

#include "Core/Core.hpp"

namespace tl::fe {
  enum class EToken {
    // Single
    LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace, RightBrace, Hash,
    Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus, FwdSlash, Percent,
    Exclaim, Equal, Greater, Less, SQuote, DQuote, QMark, Hat, Tilde, Dollar, At,
    AnnonymousIdentifier,

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
    Let, Mutable, Fn, Concept, Satisfies, Type, Global, Enum, Flag,
    For, Return, Match,
    By, Of, In, When,
    True, False,
    Class, Self, Public, Private,

    // Misc
    Empty, MaybeOperator, Reserved, Invalid,
  };

  enum class EKeyword {
  };

  class Token final {
  public:
    Token(EToken type, String str, sz line, sz column);

    static auto isValidOperator(StringView symbol) -> bool;

    auto type() const noexcept -> EToken {
      return m_type;
    }

    auto string() const noexcept -> CRef<String> {
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
    String m_str;
    sz m_line;
    sz m_column;
  };

  using Tokens = std::vector<Token>;
}

#endif // TOYLANG_FRONTEND_TOKEN_HPP
