export module lexeme;

import core;

namespace tlc::lexeme {
    export class Lexeme final {
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

        auto constexpr operator!=(Lexeme const& other) const -> b8 {
            return !(*this == other);
        }

        [[nodiscard]] auto constexpr type() const -> EType { return m_type; }
        [[nodiscard]] auto constexpr str() const -> StrV { return m_str; }

    private:
        EType m_type;
        Str m_str;
    };

    // misc
    export constexpr Lexeme empty{Lexeme::Empty, ""};
    export constexpr Lexeme invalid{Lexeme::Invalid, ""};

    // module
    export constexpr Lexeme module_{Lexeme::Module, "module"};
    export constexpr Lexeme import_{Lexeme::Import, "import"};

    // visibility
    export constexpr Lexeme pub{Lexeme::Pub, "pub"};
    export constexpr Lexeme prv{Lexeme::Prv, "prv"};

    // storage
    export constexpr Lexeme isolated{Lexeme::Isolated, "isolated"};
    export constexpr Lexeme static_{Lexeme::Static, "static"};

    // definition
    export constexpr Lexeme let{Lexeme::Let, "let"};
    export constexpr Lexeme fn{Lexeme::Fn, "fn"};
    export constexpr Lexeme trait{Lexeme::Trait, "trait"};
    export constexpr Lexeme type{Lexeme::Type, "type"};
    export constexpr Lexeme enum_{Lexeme::Enum, "enum"};
    export constexpr Lexeme flag{Lexeme::Flag, "flag"};

    // control
    export constexpr Lexeme for_{Lexeme::For, "for"};
    export constexpr Lexeme match{Lexeme::Match, "match"};
    export constexpr Lexeme return_{Lexeme::Return, "return"};
    export constexpr Lexeme defer{Lexeme::Defer, "defer"};
    export constexpr Lexeme break_{Lexeme::Break, "break"};
    export constexpr Lexeme continue_{Lexeme::Continue, "continue"};
    export constexpr Lexeme try_{Lexeme::Try, "try"};

    // adverb
    export constexpr Lexeme in{Lexeme::In, "in"};
    export constexpr Lexeme when{Lexeme::When, "when"};
    export constexpr Lexeme impl{Lexeme::Impl, "impl"};

    // reserved
    export constexpr Lexeme self{Lexeme::Self, "self"};
    export constexpr Lexeme main_{Lexeme::Main, "main"};

    // boolean
    export constexpr Lexeme true_{Lexeme::True, "true"};
    export constexpr Lexeme false_{Lexeme::False, "false"};

    // literals
    export constexpr Lexeme identifier{Lexeme::Identifier, ""};
    export constexpr Lexeme fundamentalType{Lexeme::FundamentalType, ""};
    export constexpr Lexeme userDefinedType{Lexeme::UserDefinedType, ""};
    export constexpr Lexeme integer2Literal{Lexeme::Integer2Literal, ""};
    export constexpr Lexeme integer8Literal{Lexeme::Integer8Literal, ""};
    export constexpr Lexeme integer10Literal{Lexeme::Integer10Literal, ""};
    export constexpr Lexeme integer16Literal{Lexeme::Integer16Literal, ""};
    export constexpr Lexeme floatLiteral{Lexeme::FloatLiteral, ""};
    export constexpr Lexeme stringFragment{Lexeme::StringFragment, ""};
    export constexpr Lexeme stringPlaceholder{Lexeme::StringPlaceholder, ""};

    // one-character symbols
    export constexpr Lexeme leftParen{Lexeme::LeftParen, "("};
    export constexpr Lexeme rightParen{Lexeme::RightParen, ")"};
    export constexpr Lexeme leftBracket{Lexeme::LeftBracket, "["};
    export constexpr Lexeme rightBracket{Lexeme::RightBracket, "]"};
    export constexpr Lexeme leftBrace{Lexeme::LeftBrace, "{"};
    export constexpr Lexeme rightBrace{Lexeme::RightBrace, "}"};
    export constexpr Lexeme hash{Lexeme::Hash, "#"};
    export constexpr Lexeme dot{Lexeme::Dot, "."};
    export constexpr Lexeme comma{Lexeme::Comma, ","};
    export constexpr Lexeme colon{Lexeme::Colon, ":"};
    export constexpr Lexeme semicolon{Lexeme::Semicolon, ";"};
    export constexpr Lexeme star{Lexeme::Star, "*"};
    export constexpr Lexeme ampersand{Lexeme::Ampersand, "&"};
    export constexpr Lexeme bar{Lexeme::Bar, "|"};
    export constexpr Lexeme plus{Lexeme::Plus, "+"};
    export constexpr Lexeme minus{Lexeme::Minus, "-"};
    export constexpr Lexeme fwdSlash{Lexeme::FwdSlash, "/"};
    export constexpr Lexeme percent{Lexeme::Percent, "%"};
    export constexpr Lexeme exclaim{Lexeme::Exclaim, "!"};
    export constexpr Lexeme equal{Lexeme::Equal, "="};
    export constexpr Lexeme greater{Lexeme::Greater, ">"};
    export constexpr Lexeme less{Lexeme::Less, "<"};
    export constexpr Lexeme qMark{Lexeme::QMark, "?"};
    export constexpr Lexeme hat{Lexeme::Hat, "^"};
    export constexpr Lexeme tilde{Lexeme::Tilde, "~"};
    export constexpr Lexeme dollar{Lexeme::Dollar, "$"};
    export constexpr Lexeme at{Lexeme::At, "@"};
    export constexpr Lexeme anonymous{Lexeme::Anonymous, "_"};

    // two-character symbols
    export constexpr Lexeme star2{Lexeme::Star2, "**"};
    export constexpr Lexeme ampersand2{Lexeme::Ampersand2, "&&"};
    export constexpr Lexeme bar2{Lexeme::Bar2, "||"};
    export constexpr Lexeme plus2{Lexeme::Plus2, "++"};
    export constexpr Lexeme minus2{Lexeme::Minus2, "--"};
    export constexpr Lexeme exclaim2{Lexeme::Exclaim2, "!!"};
    export constexpr Lexeme equal2{Lexeme::Equal2, "=="};
    export constexpr Lexeme greater2{Lexeme::Greater2, ">>"};
    export constexpr Lexeme less2{Lexeme::Less2, "<<"};
    export constexpr Lexeme qMark2{Lexeme::QMark2, "??"};
    export constexpr Lexeme exclaimEqual{Lexeme::ExclaimEqual, "!="};
    export constexpr Lexeme starEqual{Lexeme::StarEqual, "*="};
    export constexpr Lexeme ampersandEqual{Lexeme::AmpersandEqual, "&="};
    export constexpr Lexeme barEqual{Lexeme::BarEqual, "|="};
    export constexpr Lexeme fwdSlashEqual{Lexeme::FwdSlashEqual, "/="};
    export constexpr Lexeme percentEqual{Lexeme::PercentEqual, "%="};
    export constexpr Lexeme greaterEqual{Lexeme::GreaterEqual, ">="};
    export constexpr Lexeme lessEqual{Lexeme::LessEqual, "<="};
    export constexpr Lexeme plusEqual{Lexeme::PlusEqual, "+="};
    export constexpr Lexeme minusEqual{Lexeme::MinusEqual, "-="};
    export constexpr Lexeme colonEqual{Lexeme::ColonEqual, ":="};
    export constexpr Lexeme barGreater{Lexeme::BarGreater, "|>"};
    export constexpr Lexeme minusGreater{Lexeme::MinusGreater, "->"};
    export constexpr Lexeme lessMinus{Lexeme::LessMinus, "<-"};
    export constexpr Lexeme dot2{Lexeme::Dot2, ".."};
    export constexpr Lexeme hatEqual{Lexeme::HatEqual, "^="};
    export constexpr Lexeme equalGreater{Lexeme::EqualGreater, "=>"};

    // three-character symbols
    export constexpr Lexeme dot3{Lexeme::Dot3, "..."};
    export constexpr Lexeme greater2Equal{Lexeme::Greater2Equal, ">>="};
    export constexpr Lexeme less2Equal{Lexeme::Less2Equal, "<<="};
    export constexpr Lexeme star2Equal{Lexeme::Star2Equal, "**="};

    export const HashMap<StrV, Lexeme> nonTypeKeywordTable = {
        /* Module */{module_.str(), module_}, {import_.str(), import_},

        /* Visibility */ {pub.str(), pub}, {prv.str(), prv},

        /* Storage */ {isolated.str(), isolated}, {static_.str(), static_},

        /* Definition */{let.str(), let}, {fn.str(), fn}, {trait.str(), trait},
        {type.str(), type}, {enum_.str(), enum_}, {flag.str(), flag},

        /* Control */{for_.str(), for_}, {return_.str(), return_},
        {match.str(), match}, {defer.str(), defer}, {break_.str(), break_},
        {continue_.str(), continue_}, {try_.str(), try_},

        /* Adverb */{in.str(), in}, {when.str(), when}, {impl.str(), impl},

        /* Reserved */{self.str(), self}, {main_.str(), main_},

        /* Boolean */{true_.str(), true_}, {false_.str(), false_},
    };

    export const HashMap<StrV, Lexeme> symbolTable = {
        /* Triple characters */
        {greater2Equal.str(), greater2Equal},
        {less2Equal.str(), less2Equal},
        {star2Equal.str(), star2Equal},
        {dot3.str(), dot3},

        /* Double characters */
        {exclaimEqual.str(), exclaimEqual},
        {starEqual.str(), starEqual},
        {ampersandEqual.str(), ampersandEqual},
        {barEqual.str(), barEqual},
        {hatEqual.str(), hatEqual},
        {fwdSlashEqual.str(), fwdSlashEqual},
        {percentEqual.str(), percentEqual},
        {greaterEqual.str(), greaterEqual},
        {lessEqual.str(), lessEqual},
        {plusEqual.str(), plusEqual},
        {minusEqual.str(), minusEqual},
        {colonEqual.str(), colonEqual},
        {barGreater.str(), barGreater},
        {minusGreater.str(), minusGreater},
        {equalGreater.str(), equalGreater},
        {star2.str(), star2},
        {ampersand2.str(), ampersand2},
        {bar2.str(), bar2},
        {plus2.str(), plus2},
        {minus2.str(), minus2},
        {equal2.str(), equal2},
        {greater2.str(), greater2},
        {less2.str(), less2},
        {qMark2.str(), qMark2},
        {dot2.str(), dot2},

        /* Single character */
        {leftParen.str(), leftParen},
        {rightParen.str(), rightParen},
        {leftBracket.str(), leftBracket},
        {rightBracket.str(), rightBracket},
        {leftBrace.str(), leftBrace},
        {rightBrace.str(), rightBrace},
        {dot.str(), dot},
        {comma.str(), comma},
        {colon.str(), colon},
        {semicolon.str(), semicolon},
        {star.str(), star},
        {ampersand.str(), ampersand},
        {hash.str(), hash},
        {bar.str(), bar},
        {hat.str(), hat},
        {plus.str(), plus},
        {minus.str(), minus},
        {fwdSlash.str(), fwdSlash},
        {percent.str(), percent},
        {exclaim.str(), exclaim},
        {equal.str(), equal},
        {greater.str(), greater},
        {less.str(), less},
        {qMark.str(), qMark},
        {tilde.str(), tilde},
        {dollar.str(), dollar},
        {at.str(), at},
    };

    export const HashSet<StrV> fundamentalTypes = {
        "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
        "Own", "Ref", "Obs",
    };

    export using OpGraph3 = HashMap<c8, HashMap<c8, HashSet<c8>>>;

    export const OpGraph3 opGraph = [] {
        OpGraph3 graph;
        rng::for_each(
            symbolTable
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
}

template <>
struct std::hash<tlc::lexeme::Lexeme> {
    constexpr auto operator()(
        tlc::lexeme::Lexeme const& lexeme
    ) const noexcept -> tlc::szt {
        return hash<tlc::szt>()(static_cast<tlc::szt>(lexeme.type()));
    }
};
