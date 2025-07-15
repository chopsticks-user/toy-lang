#ifndef TLC_TOKEN_HPP
#define TLC_TOKEN_HPP

#include "core/core.hpp"

namespace tlc::token {
    enum class EToken {
        Invalid,

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
    };

    enum class EKeyword {};

    class Token final {
    public:
        Token(EToken const type, StrV const str, szt const line, szt const column)
            : m_type{type}, m_str{str},
              m_line{line}, m_column{column} {}

        [[nodiscard]] auto type() const noexcept -> EToken {
            return m_type;
        }

        [[nodiscard]] auto str() const noexcept -> StrV {
            return m_str;
        }

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_column;
        }

    private:
        EToken m_type;
        Str m_str;
        szt m_line;
        szt m_column;
    };
}

#endif // TLC_TOKEN_HPP
