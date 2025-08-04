#ifndef TLC_TOKEN_CLASSIFY_HPP
#define TLC_TOKEN_CLASSIFY_HPP

#include "core/core.hpp"

namespace tlc::lexeme {
    class Lexeme final {
    public: // private is fine, added to hide linter errors
        enum class EType {
            // Misc
            Empty, Invalid,

            // Keywords
            /* Module */ Module, Import,
            /* Visibility */ Pub, Prv,
            /* Storage */ Isolated, Static,
            /* Definition */ Let, Fn, Trait, Type, Enum, Flag, Try,
            /* Control */ For, Match, Return, Defer, Preface, Break, Continue,
            /* Adverb */ In, When, Impl,
            /* Reserved */ Self, Main,
            /* Boolean */ True, False,

            // Literals
            Identifier, FundamentalType, UserDefinedType,
            Integer2Literal, Integer8Literal, Integer10Literal, Integer16Literal,
            FloatLiteral, StringFragment, StringPlaceholder,

            // Symbols
            /* Single character */ LeftParen, RightParen, LeftBracket, RightBracket, LeftBrace,
            RightBrace, Hash, Dot, Comma, Colon, Semicolon, Star, Ampersand, Bar, Plus, Minus,
            FwdSlash, Percent, Exclaim, Equal, Greater, Less, QMark, Hat, Tilde,
            Dollar, At, Anonymous,
            /* Double characters */ Star2, Ampersand2, Bar2, Plus2, Minus2, Exclaim2,
            Equal2, Greater2, Less2, QMark2, ExclaimEqual, StarEqual, AmpersandEqual, BarEqual,
            FwdSlashEqual, PercentEqual, GreaterEqual, LessEqual, PlusEqual, MinusEqual,
            ColonEqual, BarGreater, MinusGreater, LessMinus, Dot2, HatEqual, EqualGreater,
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

    // misc
    constexpr Lexeme empty{Lexeme::Empty, ""};
    constexpr Lexeme invalid{Lexeme::Invalid, ""};

    // module
    constexpr Lexeme module{Lexeme::Module, "module"};
    constexpr Lexeme import_{Lexeme::Import, "import"};

    // visibility
    constexpr Lexeme pub{Lexeme::Pub, "pub"};
    constexpr Lexeme prv{Lexeme::Prv, "prv"};

    // storage
    constexpr Lexeme isolated{Lexeme::Isolated, "isolated"};
    constexpr Lexeme static_{Lexeme::Static, "static"};

    // definition
    constexpr Lexeme let{Lexeme::Let, "let"};
    constexpr Lexeme fn{Lexeme::Fn, "fn"};
    constexpr Lexeme trait{Lexeme::Trait, "trait"};
    constexpr Lexeme type{Lexeme::Type, "type"};
    constexpr Lexeme enum_{Lexeme::Enum, "enum"};
    constexpr Lexeme flag{Lexeme::Flag, "flag"};

    // control
    constexpr Lexeme for_{Lexeme::For, "for"};
    constexpr Lexeme match{Lexeme::Match, "match"};
    constexpr Lexeme return_{Lexeme::Return, "return"};
    constexpr Lexeme defer{Lexeme::Defer, "defer"};
    constexpr Lexeme break_{Lexeme::Break, "break"};
    constexpr Lexeme continue_{Lexeme::Continue, "continue"};
    constexpr Lexeme try_{Lexeme::Try, "try"};

    // adverb
    constexpr Lexeme in{Lexeme::In, "in"};
    constexpr Lexeme when{Lexeme::When, "when"};
    constexpr Lexeme impl{Lexeme::Impl, "impl"};

    // reserved
    constexpr Lexeme self{Lexeme::Self, "self"};
    constexpr Lexeme main_{Lexeme::Main, "main"};

    // boolean
    constexpr Lexeme true_{Lexeme::True, "true"};
    constexpr Lexeme false_{Lexeme::False, "false"};

    // literals
    constexpr Lexeme identifier{Lexeme::Identifier, ""};
    constexpr Lexeme fundamentalType{Lexeme::FundamentalType, ""};
    constexpr Lexeme userDefinedType{Lexeme::UserDefinedType, ""};
    constexpr Lexeme integer2Literal{Lexeme::Integer2Literal, ""};
    constexpr Lexeme integer8Literal{Lexeme::Integer8Literal, ""};
    constexpr Lexeme integer10Literal{Lexeme::Integer10Literal, ""};
    constexpr Lexeme integer16Literal{Lexeme::Integer16Literal, ""};
    constexpr Lexeme floatLiteral{Lexeme::FloatLiteral, ""};
    constexpr Lexeme stringFragment{Lexeme::StringFragment, ""};
    constexpr Lexeme stringPlaceholder{Lexeme::StringPlaceholder, ""};

    // one-character symbols
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
    constexpr Lexeme qMark{Lexeme::QMark, "?"};
    constexpr Lexeme hat{Lexeme::Hat, "^"};
    constexpr Lexeme tilde{Lexeme::Tilde, "~"};
    constexpr Lexeme dollar{Lexeme::Dollar, "$"};
    constexpr Lexeme at{Lexeme::At, "@"};
    constexpr Lexeme anonymous{Lexeme::Anonymous, "_"};

    // two-character symbols
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
    constexpr Lexeme colonEqual{Lexeme::ColonEqual, ":="};
    constexpr Lexeme barGreater{Lexeme::BarGreater, "|>"};
    constexpr Lexeme minusGreater{Lexeme::MinusGreater, "->"};
    constexpr Lexeme lessMinus{Lexeme::LessMinus, "<-"};
    constexpr Lexeme dot2{Lexeme::Dot2, ".."};
    constexpr Lexeme hatEqual{Lexeme::HatEqual, "^="};
    constexpr Lexeme equalGreater{Lexeme::EqualGreater, "=>"};

    // three-character symbols
    constexpr Lexeme dot3{Lexeme::Dot3, "..."};
    constexpr Lexeme greater2Equal{Lexeme::Greater2Equal, ">>="};
    constexpr Lexeme less2Equal{Lexeme::Less2Equal, "<<="};
    constexpr Lexeme star2Equal{Lexeme::Star2Equal, "**="};

    extern const HashMap<StrV, Lexeme> nonTypeKeywordTable;
    extern const HashMap<StrV, Lexeme> symbolTable;
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
