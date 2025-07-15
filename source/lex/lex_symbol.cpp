#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lexer::lexSymbol() -> void {
        if (isUnnamedIdentifier(m_stream.current())) {
            m_currentTokenType = token::EToken::AnonymousIdentifier;
            appendToken();
            return;
        }

        // todo: string

        if (auto const entry = token::opGraph.find(m_stream.current());
            entry != token::opGraph.end()) {
            if (auto const entry2 = entry->second.find(m_stream.peek());
                entry2 != entry->second.end()) {
                m_stream.advance();
                appendLexeme();
                if (entry2->second.contains(m_stream.peek())) {
                    m_stream.advance();
                    appendLexeme();
                }
            }
        }
        m_currentTokenType = token::operatorTable.at(m_currentLexeme);
        appendToken();
    }
}
