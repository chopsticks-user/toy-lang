#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::classifyIdentifier(StrV const lexeme)
        -> void {
        if (isLowerCaseLetter(lexeme.front())) {
            if (lexeme::nonTypeKeywordTable.contains(lexeme)) {
                m_currentLexeme = lexeme::nonTypeKeywordTable.at(lexeme);
            }
            else {
                m_currentLexeme = lexeme::identifier;
            }
        }
        else {
            if (lexeme::fundamentalTypes.contains(lexeme)) {
                m_currentLexeme = lexeme::fundamentalType;
            }
            else {
                m_currentLexeme = lexeme::userDefinedType;
            }
        }
    }

    auto Lex::lexIdentifier() -> void {
        while (m_stream.match(isDigitOrLetter)) {
            appendStr();
        }
        classifyIdentifier(m_currentStr);
        appendToken();
    }
}
