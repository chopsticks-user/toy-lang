#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::classifyIdentifier(StrV const lexeme)
        -> void {
        using enum token::EToken;

        if (isLowerCaseLetter(lexeme.front())) {
            if (token::reservedKeywords.contains(lexeme)) {
                m_currentTokenType = Invalid;
            }
            else if (token::nonTypeKeywordTable.contains(lexeme)) {
                m_currentTokenType = token::nonTypeKeywordTable.at(lexeme);
            }
            else {
                m_currentTokenType = Identifier;
            }
        }
        else {
            if (token::fundamentalTypes.contains(lexeme)) {
                m_currentTokenType = FundamentalType;
            }
            else {
                m_currentTokenType = UserDefinedType;
            }
        }
    }

    auto Lex::lexIdentifier() -> void {
        while (m_stream.match(isDigitOrLetter)) {
            appendLexeme();
        }
        classifyIdentifier(m_currentLexeme);
        appendToken();
    }
}
