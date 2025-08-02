#ifndef TLC_TOKEN_CLASSIFY_HPP
#define TLC_TOKEN_CLASSIFY_HPP

#include "core/core.hpp"

namespace tlc::lexeme {
    class Lexeme final {
    public: // private is fine, added to hide linter errors
        enum class EType {
            Invalid,

            // Keywords
            /* Visibility */ Module, Import, Export, Internal, Local, Extern, Static,
            /* Definition */ Let, Fn, Trait, Type, Enum, Flag,
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
            /* Triple characters */ Dot3, Greater2Equal, Less2Equal, Star2Equal,
        };

    public:
        using enum EType;

        explicit constexpr Lexeme(EType const type, StrV const str = "")
            : m_type{type}, m_str{str} {}

        auto constexpr operator==(Lexeme const& other) const -> b8 {
            return m_type == other.m_type;
        }

        auto constexpr operator==(Lexeme&& other) const -> b8 {
            return m_type == other.m_type;
        }

        auto constexpr operator!=(Lexeme const& other) const -> b8 {
            return !(*this == other);
        }

        auto constexpr operator!=(Lexeme&& other) const -> b8 {
            return !(*this != other);
        }

        [[nodiscard]] auto constexpr type() const -> EType { return m_type; }
        [[nodiscard]] auto constexpr str() const -> StrV { return m_str; }

    private:
        EType m_type;
        Str m_str;
    };

    constexpr Lexeme invalid{Lexeme::Invalid, ""};
    constexpr Lexeme module{Lexeme::Module, "module"};
    constexpr Lexeme import_{Lexeme::Import, "import"};
    constexpr Lexeme export_{Lexeme::Export, "export"};
    constexpr Lexeme internal{Lexeme::Internal, "internal"};
    constexpr Lexeme local{Lexeme::Local, "local"};
    constexpr Lexeme extern_{Lexeme::Extern, "extern"};
    constexpr Lexeme static_{Lexeme::Static, "static"};
    constexpr Lexeme let{Lexeme::Let, "let"};
    constexpr Lexeme fn{Lexeme::Fn, "fn"};
    constexpr Lexeme trait{Lexeme::Trait, "trait"};
    constexpr Lexeme type{Lexeme::Type, "type"};
    constexpr Lexeme enum_{Lexeme::Enum, "enum"};
    constexpr Lexeme flag{Lexeme::Flag, "flag"};
    constexpr Lexeme for_{Lexeme::For, "for"};
    constexpr Lexeme match{Lexeme::Match, "match"};
    constexpr Lexeme return_{Lexeme::Return, "return"};
    constexpr Lexeme yield{Lexeme::Yield, "yield"};
    constexpr Lexeme defer{Lexeme::Defer, "defer"};
    constexpr Lexeme preface{Lexeme::Preface, "preface"};
    constexpr Lexeme by{Lexeme::By, "by"};
    constexpr Lexeme of{Lexeme::Of, "of"};
    constexpr Lexeme in{Lexeme::In, "in"};
    constexpr Lexeme when{Lexeme::When, "when"};
    constexpr Lexeme true_{Lexeme::True, "true"};
    constexpr Lexeme false_{Lexeme::False, "false"};
    constexpr Lexeme self{Lexeme::Self, "self"};
    constexpr Lexeme pub{Lexeme::Pub, "pub"};
    constexpr Lexeme prv{Lexeme::Prv, "prv"};
    constexpr Lexeme impl{Lexeme::Impl, "impl"};
    constexpr Lexeme identifier{Lexeme::Identifier, ""};
    constexpr Lexeme fundamentalType{Lexeme::FundamentalType, ""};
    constexpr Lexeme userDefinedType{Lexeme::UserDefinedType, ""};
    constexpr Lexeme integer2Literal{Lexeme::Integer2Literal, ""};
    constexpr Lexeme integer8Literal{Lexeme::Integer8Literal, ""};
    constexpr Lexeme integer10Literal{Lexeme::Integer10Literal, ""};
    constexpr Lexeme integer16Literal{Lexeme::Integer16Literal, ""};
    constexpr Lexeme floatLiteral{Lexeme::FloatLiteral, ""};
    constexpr Lexeme stringLiteral{Lexeme::StringLiteral, ""};
    constexpr Lexeme leftParen{Lexeme::LeftParen, "("};
    constexpr Lexeme rightParen{Lexeme::RightParen, ")"};
    constexpr Lexeme leftBracket{Lexeme::LeftBracket, "["};
    constexpr Lexeme rightBracket{Lexeme::RightBracket, "]"};
    constexpr Lexeme leftBrace{Lexeme::LeftBrace, "{"};
    constexpr Lexeme rightBrace{Lexeme::RightBrace, "}"};
    constexpr Lexeme hash{Lexeme::Hash, "#"};
    constexpr Lexeme dot{Lexeme::Dot, "."};
    constexpr Lexeme comma{Lexeme::Comma, ","};
    constexpr Lexeme colon{Lexeme::Colon, ":"};
    constexpr Lexeme semicolon{Lexeme::Semicolon, ";"};
    constexpr Lexeme star{Lexeme::Star, "*"};
    constexpr Lexeme ampersand{Lexeme::Ampersand, "&"};
    constexpr Lexeme bar{Lexeme::Bar, "|"};
    constexpr Lexeme plus{Lexeme::Plus, "+"};
    constexpr Lexeme minus{Lexeme::Minus, "-"};
    constexpr Lexeme fwdSlash{Lexeme::FwdSlash, "/"};
    constexpr Lexeme percent{Lexeme::Percent, "%"};
    constexpr Lexeme exclaim{Lexeme::Exclaim, "!"};
    constexpr Lexeme equal{Lexeme::Equal, "="};
    constexpr Lexeme greater{Lexeme::Greater, ">"};
    constexpr Lexeme less{Lexeme::Less, "<"};
    constexpr Lexeme sQuote{Lexeme::SQuote, "'"};
    constexpr Lexeme dQuote{Lexeme::DQuote, "\""};
    constexpr Lexeme qMark{Lexeme::QMark, "?"};
    constexpr Lexeme hat{Lexeme::Hat, "^"};
    constexpr Lexeme tilde{Lexeme::Tilde, "~"};
    constexpr Lexeme dollar{Lexeme::Dollar, "$"};
    constexpr Lexeme at{Lexeme::At, "@"};
    constexpr Lexeme anonymousIdentifier{Lexeme::AnonymousIdentifier, "_"};
    constexpr Lexeme colon2{Lexeme::Colon2, "::"};
    constexpr Lexeme star2{Lexeme::Star2, "**"};
    constexpr Lexeme ampersand2{Lexeme::Ampersand2, "&&"};
    constexpr Lexeme bar2{Lexeme::Bar2, "||"};
    constexpr Lexeme plus2{Lexeme::Plus2, "++"};
    constexpr Lexeme minus2{Lexeme::Minus2, "--"};
    constexpr Lexeme exclaim2{Lexeme::Exclaim2, "!!"};
    constexpr Lexeme equal2{Lexeme::Equal2, "=="};
    constexpr Lexeme greater2{Lexeme::Greater2, ">>"};
    constexpr Lexeme less2{Lexeme::Less2, "<<"};
    constexpr Lexeme qMark2{Lexeme::QMark2, "??"};
    constexpr Lexeme exclaimEqual{Lexeme::ExclaimEqual, "!="};
    constexpr Lexeme starEqual{Lexeme::StarEqual, "*="};
    constexpr Lexeme ampersandEqual{Lexeme::AmpersandEqual, "&="};
    constexpr Lexeme barEqual{Lexeme::BarEqual, "|="};
    constexpr Lexeme fwdSlashEqual{Lexeme::FwdSlashEqual, "/="};
    constexpr Lexeme percentEqual{Lexeme::PercentEqual, "%="};
    constexpr Lexeme greaterEqual{Lexeme::GreaterEqual, ">="};
    constexpr Lexeme lessEqual{Lexeme::LessEqual, "<="};
    constexpr Lexeme plusEqual{Lexeme::PlusEqual, "+="};
    constexpr Lexeme minusEqual{Lexeme::MinusEqual, "-="};
    constexpr Lexeme barGreater{Lexeme::BarGreater, "|>"};
    constexpr Lexeme minusGreater{Lexeme::MinusGreater, "->"};
    constexpr Lexeme lessMinus{Lexeme::LessMinus, "<-"};
    constexpr Lexeme dot2{Lexeme::Dot2, ".."};
    constexpr Lexeme hatEqual{Lexeme::HatEqual, "^="};
    constexpr Lexeme equalGreater{Lexeme::EqualGreater, "=>"};
    constexpr Lexeme dot3{Lexeme::Dot3, "..."};
    constexpr Lexeme greater2Equal{Lexeme::Greater2Equal, ">>="};
    constexpr Lexeme less2Equal{Lexeme::Less2Equal, "<<="};
    constexpr Lexeme star2Equal{Lexeme::Star2Equal, "**="};

    extern const HashMap<StrV, Lexeme> nonTypeKeywordTable;
    extern const HashMap<StrV, Lexeme> operatorTable;
    extern const HashSet<StrV> fundamentalTypes;
    using OpGraph3 = HashMap<c8, HashMap<c8, HashSet<c8>>>;
    extern const OpGraph3 opGraph;
}

template <>
struct std::hash<tlc::lexeme::Lexeme> {
    constexpr auto operator()(
        tlc::lexeme::Lexeme const& lexeme
    ) const noexcept -> tlc::szt {
        return hash<tlc::szt>()(static_cast<tlc::szt>(lexeme.type()));
    }
};

#endif // TLC_TOKEN_CLASSIFY_HPP
