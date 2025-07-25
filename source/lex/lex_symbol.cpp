#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::lexSymbol() -> void {
        appendLexeme();
        if (isUnnamedIdentifier(m_stream.current())) {
            m_currentTokenType = token::EToken::AnonymousIdentifier;
            appendToken();
            return;
        }

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

        if (m_currentLexeme.empty()) {
            // todo: error
            m_stream.advance();
            return;
        }

        m_currentTokenType = token::operatorTable.at(m_currentLexeme);
        appendToken();
    }
}
