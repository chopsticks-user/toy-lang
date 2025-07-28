#ifndef TLC_TOKEN_CLASSIFY_HPP
#define TLC_TOKEN_CLASSIFY_HPP

#include "core/core.hpp"

namespace tlc::token {
    using OpGraph3 = HashMap<char, HashMap<char, HashSet<char>>>;

    enum class EToken {
        Invalid,

        // Keywords
        /* Visibility */ Module, Import, Export, Internal, Local, Extern, Static,
        /* Definition */ Let, Mut, Fn, Trait, Type, Enum, Flag,
        /* Control */ For, Match, Return, Yield, Defer, Preface,
        /* Adverb */ By, Of, In, When,
        /* Boolean */ True, False,
        /* Object */ Self, Pub, Prv, Impl,

        // Identifiers
        Identifier, FundamentalType, UserDefinedType,

        // Literals
        Integer2Literal, Integer8Literal, Integer10Literal, Integer16Literal,
        FloatLiteral, StringLiteral,

        // Symbols
        /* Single character */ LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace,
        RightBrace, Hash, Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus,
        FwdSlash, Percent, Exclaim, Equal, Greater, Less, SQuote, DQuote, QMark, Hat, Tilde,
        Dollar, At, AnonymousIdentifier,
        /* Double characters */ Colon2, Star2, Ampersand2, Bar2, Plus2, Minus2, Exclaim2,
        Equal2, Greater2, Less2, QMark2, ExclaimEqual, StarEqual, AmpersandEqual, BarEqual,
        FwdSlashEqual, PercentEqual, GreaterEqual, LessEqual, PlusEqual, MinusEqual, BarGreater,
        MinusGreater, LessMinus, Dot2, HatEqual, EqualGreater,
        /* Triple characters */ Dot3, Greater2Equal, Less2Equal, ColonEqualGreater,
        ColonTildeGreater, Star2Equal,
    };

    extern const HashMap<StrV, EToken> nonTypeKeywordTable;

    extern const HashSet<StrV> fundamentalTypes;

    extern const HashSet<StrV> reservedKeywords;

    extern const HashMap<StrV, EToken> operatorTable;

    extern const OpGraph3 opGraph;
}

#endif // TLC_TOKEN_CLASSIFY_HPP
