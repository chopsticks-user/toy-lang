#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    static const HashMap<StrV, token::EToken> nonTypeKeywordTable = {
        // visibility
        {"module", token::EToken::Module},
        {"import", token::EToken::Import},
        {"export", token::EToken::Export},
        {"internal", token::EToken::Internal},
        {"local", token::EToken::Local},
        {"extern", token::EToken::Extern},

        // definition
        {"let", token::EToken::Let},
        {"mut", token::EToken::Mut},
        {"fn", token::EToken::Fn},
        {"trait", token::EToken::Trait},
        {"type", token::EToken::Type},
        {"enum", token::EToken::Enum},
        {"flag", token::EToken::Flag},
        // {"pure", token::EToken::Pure},

        // control
        {"for", token::EToken::For},
        {"return", token::EToken::Return},
        {"match", token::EToken::Match},

        // adverb
        {"by", token::EToken::By},
        {"of", token::EToken::Of},
        {"in", token::EToken::In},
        {"when", token::EToken::When},

        // boolean
        {"true", token::EToken::True},
        {"false", token::EToken::False},

        // object-oriented
        {"self", token::EToken::Self},
        {"pub", token::EToken::Pub},
        {"prv", token::EToken::Prv},
        {"impl", token::EToken::Impl},
    };

    static const HashSet<StrV> fundamentalTypes{
        "Int", "Float", "Bool", "Char", "Void", "String", "Any",
        // "Rxt", "Atm", "Rng"
    };

    static const HashSet<StrV> reservedKeywords{
        "int", "float", "bool", "char", "void", "string", "any"
    };

    static auto classifyLowercaseIdentifier(StrV const lexeme)
        -> token::EToken {
        using enum token::EToken;

        if (reservedKeywords.contains(lexeme)) {
            return Reserved;
        }
        if (nonTypeKeywordTable.contains(lexeme)) {
            return nonTypeKeywordTable.at(lexeme);
        }
        return Identifier;
    }

    static auto classifyUppercaseIdentifier(StrV const lexeme)
        -> token::EToken {
        using enum token::EToken;

        if (fundamentalTypes.contains(lexeme)) {
            return FundamentalType;
        }
        return UserDefinedType;
    }

    static auto classifyIdentifier(StrV lexeme)
        -> token::EToken {
        if (isLowerCaseLetter(lexeme.front())) {
            return classifyLowercaseIdentifier(std::move(lexeme));
        }
        return classifyUppercaseIdentifier(std::move(lexeme));
    }

    auto Lexer::lexIdentifier() -> void {
        while (m_stream.match(isDigitOrLetter)) {
            m_currentLexeme += m_stream.current();
        }
        classifyIdentifier(m_currentLexeme);
        appendToken();
    }
}
