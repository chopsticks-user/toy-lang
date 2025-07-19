#ifndef TLC_TOKEN_HPP
#define TLC_TOKEN_HPP

#include "core/core.hpp"

namespace tlc::token {
    using OpGraph3 = HashMap<char, HashMap<char, HashSet<char>>>;

    enum class EToken {
        Invalid,

        // Keywords
        /* Visibility */ Module, Import, Export, Internal, Local, Extern, Static,
        /* Definition */ Let, Mut, Fn, Trait, Type, Enum, Flag,
        /* Control */ For, Return, Match,
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

    static const HashMap<StrV, EToken> nonTypeKeywordTable = {
        // Visibility
        {"module", EToken::Module},
        {"import", EToken::Import},
        {"export", EToken::Export},
        {"internal", EToken::Internal},
        {"local", EToken::Local},
        {"extern", EToken::Extern},

        // Definition
        {"let", EToken::Let},
        {"mut", EToken::Mut},
        {"fn", EToken::Fn},
        {"trait", EToken::Trait},
        {"type", EToken::Type},
        {"enum", EToken::Enum},
        {"flag", EToken::Flag},
        // {"pure", EToken::Pure},

        // Control
        {"for", EToken::For},
        {"return", EToken::Return},
        {"match", EToken::Match},

        // Adverb
        {"by", EToken::By},
        {"of", EToken::Of},
        {"in", EToken::In},
        {"when", EToken::When},

        // Boolean
        {"true", EToken::True},
        {"false", EToken::False},

        // Object
        {"self", EToken::Self},
        {"pub", EToken::Pub},
        {"prv", EToken::Prv},
        {"impl", EToken::Impl},
    };

    static const HashSet<StrV> fundamentalTypes{
        "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
        // "Rxt", "Atm", "Rng"
    };

    static const HashSet<StrV> reservedKeywords{
        "int", "float", "bool", "char", "void", "string", "any", "opt",
    };

    static const HashMap<StrV, EToken> operatorTable{
        // Triple characters
        {":=>", EToken::ColonEqualGreater},
        {":~>", EToken::ColonTildeGreater},
        {">>=", EToken::Greater2Equal},
        {"<<=", EToken::Less2Equal},
        {"**=", EToken::Star2Equal},
        {"...", EToken::Dot3},

        // Double characters
        {"!=", EToken::ExclaimEqual},
        {"*=", EToken::StarEqual},
        {"&=", EToken::AmpersandEqual},
        {"|=", EToken::BarEqual},
        {"^=", EToken::HatEqual},
        {"/=", EToken::FwdSlashEqual},
        {"%=", EToken::PercentEqual},
        {">=", EToken::GreaterEqual},
        {"<=", EToken::LessEqual},
        {"+=", EToken::PlusEqual},
        {"-=", EToken::MinusEqual},
        {"|>", EToken::BarGreater},
        {"->", EToken::MinusGreater},
        {"=>", EToken::EqualGreater},
        // {"<-", EToken::LessMinus},
        {"::", EToken::Colon2},
        {"**", EToken::Star2},
        {"&&", EToken::Ampersand2},
        {"||", EToken::Bar2},
        {"++", EToken::Plus2},
        {"--", EToken::Minus2},
        // {"!!", EToken::Exclaim2},
        {"==", EToken::Equal2},
        {">>", EToken::Greater2},
        {"<<", EToken::Less2},
        {"??", EToken::QMark2},
        {"..", EToken::Dot2},

        // Single character
        {"(", EToken::LeftParen},
        {")", EToken::RightParen},
        {"[", EToken::LeftBracket},
        {"]", EToken::RightBracket},
        {"{", EToken::LeftBrace},
        {"}", EToken::RightBrace},
        {".", EToken::Dot},
        {",", EToken::Comma},
        {":", EToken::Colon},
        {";", EToken::Semicolon},
        {"*", EToken::Star},
        {"&", EToken::Ampersand},
        {"#", EToken::Hash},
        {"|", EToken::Bar},
        {"^", EToken::Hat},
        {"+", EToken::Plus},
        {"-", EToken::Minus},
        {"/", EToken::FwdSlash},
        {"%", EToken::Percent},
        {"!", EToken::Exclaim},
        {"=", EToken::Equal},
        {">", EToken::Greater},
        {"<", EToken::Less},
        {"'", EToken::SQuote},
        {"\"", EToken::DQuote},
        {"?", EToken::QMark},
        {"~", EToken::Tilde},
        {"$", EToken::Dollar},
        {"@", EToken::At},
    };

    static const OpGraph3 opGraph = [] {
        OpGraph3 graph;
        rng::for_each(
            operatorTable
            | rv::transform([](auto const& entry) {
                return entry.first;
            }), [&graph](StrV const op) {
                if (!graph.contains(op[0])) {
                    graph[op[0]] = {};
                }
                if (op.length() > 1) {
                    if (!graph[op[0]].contains(op[1])) {
                        graph[op[0]][op[1]] = {};
                    }
                    if (op.length() > 2) {
                        graph[op[0]][op[1]].emplace(op[2]);
                    }
                }
            });
        return graph;
    }();

    class Token final {
    public:
        using Coords = Pair<szt, szt>;

    public:
        Token(EToken const type, StrV const str, Coords coords)
            : m_type{type}, m_str{str}, m_coords{std::move(coords)} {}

        [[nodiscard]] auto type() const noexcept -> EToken {
            return m_type;
        }

        [[nodiscard]] auto str() const noexcept -> StrV {
            return m_str;
        }

        [[nodiscard]] auto coords() const noexcept -> Coords {
            return m_coords;
        }

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_coords.first;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_coords.second;
        }

    private:
        EToken m_type;
        Str m_str;
        Coords m_coords;
    };
}

#endif // TLC_TOKEN_HPP
