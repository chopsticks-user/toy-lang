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

    auto Lexer::classifyIdentifier(StrV const lexeme)
        -> void {
        using enum token::EToken;

        if (isLowerCaseLetter(lexeme.front())) {
            if (reservedKeywords.contains(lexeme)) {
                m_currentTokenType = Invalid;
            }
            else if (nonTypeKeywordTable.contains(lexeme)) {
                m_currentTokenType = nonTypeKeywordTable.at(lexeme);
            }
            else {
                m_currentTokenType = Identifier;
            }
        }
        else {
            if (fundamentalTypes.contains(lexeme)) {
                m_currentTokenType = FundamentalType;
            }
            else {
                m_currentTokenType = UserDefinedType;
            }
        }
    }

    auto Lexer::lexIdentifier() -> void {
        while (m_stream.match(isDigitOrLetter)) {
            appendLexeme();
        }
        classifyIdentifier(m_currentLexeme);
        appendToken();
    }
}
