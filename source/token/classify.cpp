#include "classify.hpp"

namespace tlc {
    namespace lexeme {
        const HashSet<Lexeme> nonTypeKeywordTable = {
            /* Visibility */module, import_, export_, internal, local, extern_,
            /* Definition */let, fn, trait, type, enum_, flag,
            /* Control */for_, return_, yield, match, defer, preface,
            /* Adverb */by, of, in, when,
            /* Boolean */true_, false_,
            /* Object */self, pub, prv, impl,
        };

        const HashSet<Lexeme> operatorTable = {
            /* Triple characters */greater2Equal, less2Equal, star2Equal, dot3,
            /* Double characters */exclaimEqual, starEqual, ampersandEqual, barEqual,
            hatEqual, fwdSlashEqual, percentEqual, greaterEqual, lessEqual,
            plusEqual, minusEqual, barGreater, minusGreater, equalGreater, colon2,
            star2, ampersand2, bar2, plus2, minus2, equal2, greater2, less2,
            qMark2, dot2,
            /* Single character */leftParen, rightParen, leftBracket, rightBracket,
            leftBrace, rightBrace, dot, comma, colon, semicolon, star,
            ampersand, hash, bar, hat, plus, minus, fwdSlash, percent,
            exclaim, equal, greater, less, sQuote, dQuote, qMark, tilde,
            dollar, at
        };

        const HashSet<StrV> fundamentalTypes = {
            "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
        };

        const OpGraph3 opGraph = [] {
            OpGraph3 graph;
            rng::for_each(
                operatorTable
                | rv::transform([](auto const& entry) {
                    return entry.str();
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

        // {":=>", EToken::ColonEqualGreater},
        // {":~>", EToken::ColonTildeGreater},
        // {"!!", EToken::Exclaim2},
        // {"<-", EToken::LessMinus},
    }

    namespace token {
        const HashMap<StrV, EToken> nonTypeKeywordTable = {
            // Visibility
            {"module", EToken::Module},
            {"import", EToken::Import},
            {"export", EToken::Export},
            {"internal", EToken::Internal},
            {"local", EToken::Local},
            {"extern", EToken::Extern},

            // Definition
            {"let", EToken::Let},
            {"fn", EToken::Fn},
            {"trait", EToken::Trait},
            {"type", EToken::Type},
            {"enum", EToken::Enum},
            {"flag", EToken::Flag},

            // Control
            {"for", EToken::For},
            {"return", EToken::Return},
            {"yield", EToken::Yield},
            {"match", EToken::Match},
            {"defer", EToken::Defer},
            {"preface", EToken::Preface},

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

        const HashSet<StrV> fundamentalTypes = {
            "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
            // "Rxt", "Atm", "Rng"
        };

        const HashSet<StrV> reservedKeywords = {
            "int", "float", "bool", "char", "void", "string", "any", "opt",
        };

        const HashMap<StrV, EToken> operatorTable = {
            // Triple characters
            // {":=>", EToken::ColonEqualGreater},
            // {":~>", EToken::ColonTildeGreater},
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

        const HashMap<EToken, StrV> reversedOperatorTable = {
            // Triple characters
            // {EToken::ColonEqualGreater, ":=>"},
            // {EToken::ColonTildeGreater, ":~>"},
            {EToken::Greater2Equal, ">>="},
            {EToken::Less2Equal, "<<="},
            {EToken::Star2Equal, "**="},
            {EToken::Dot3, "..."},

            // Double characters
            {EToken::ExclaimEqual, "!="},
            {EToken::StarEqual, "*="},
            {EToken::AmpersandEqual, "&="},
            {EToken::BarEqual, "|="},
            {EToken::HatEqual, "^="},
            {EToken::FwdSlashEqual, "/="},
            {EToken::PercentEqual, "%="},
            {EToken::GreaterEqual, ">="},
            {EToken::LessEqual, "<="},
            {EToken::PlusEqual, "+="},
            {EToken::MinusEqual, "-="},
            {EToken::BarGreater, "|>"},
            {EToken::MinusGreater, "->"},
            {EToken::EqualGreater, "=>"},
            {EToken::Colon2, "::"},
            {EToken::Star2, "**"},
            {EToken::Ampersand2, "&&"},
            {EToken::Bar2, "||"},
            {EToken::Plus2, "++"},
            {EToken::Minus2, "--"},
            {EToken::Equal2, "=="},
            {EToken::Greater2, ">>"},
            {EToken::Less2, "<<"},
            {EToken::QMark2, "??"},
            {EToken::Dot2, ".."},

            // Single character
            {EToken::LeftParen, "("},
            {EToken::RightParen, ")"},
            {EToken::LeftBracket, "["},
            {EToken::RightBracket, "]"},
            {EToken::LeftBrace, "{"},
            {EToken::RightBrace, "}"},
            {EToken::Dot, "."},
            {EToken::Comma, ","},
            {EToken::Colon, ":"},
            {EToken::Semicolon, ";"},
            {EToken::Star, "*"},
            {EToken::Ampersand, "&"},
            {EToken::Hash, "#"},
            {EToken::Bar, "|"},
            {EToken::Hat, "^"},
            {EToken::Plus, "+"},
            {EToken::Minus, "-"},
            {EToken::FwdSlash, "/"},
            {EToken::Percent, "%"},
            {EToken::Exclaim, "!"},
            {EToken::Equal, "="},
            {EToken::Greater, ">"},
            {EToken::Less, "<"},
            {EToken::SQuote, "'"},
            {EToken::DQuote, "\""},
            {EToken::QMark, "?"},
            {EToken::Tilde, "~"},
            {EToken::Dollar, "$"},
            {EToken::At, "@"},
        };

        const OpGraph3 opGraph = [] {
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
    }
}
