#include "classify.hpp"

namespace tlc::token {
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

    const HashSet<StrV> fundamentalTypes = {
        "Int", "Float", "Bool", "Char", "Void", "String", "Any", "Opt",
        // "Rxt", "Atm", "Rng"
    };

    const HashSet<StrV> reservedKeywords = {
        "int", "float", "bool", "char", "void", "string", "any", "opt",
    };

    const HashMap<StrV, EToken> operatorTable = {
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

    const HashMap<EToken, StrV> reversedOperatorTable = {
        // Triple characters
        {EToken::ColonEqualGreater, ":=>"},
        {EToken::ColonTildeGreater, ":~>"},
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
