#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    auto Lex::lexSymbol() -> void {
        appendStr();
        if (isUnnamedIdentifier(m_stream.current())) {
            m_currentLexeme = lexeme::anonymous;
            appendToken();
            return;
        }

        if (auto const entry = lexeme::opGraph.find(m_stream.current());
            entry != lexeme::opGraph.end()) {
            if (auto const entry2 = entry->second.find(m_stream.peek());
                entry2 != entry->second.end()) {
                m_stream.advance();
                appendStr();
                if (entry2->second.contains(m_stream.peek())) {
                    m_stream.advance();
                    appendStr();
                }
            }
        }

        if (m_currentStr.empty()) {
            // todo: error
            m_stream.advance();
            return;
        }

        m_currentLexeme = lexeme::symbolTable.at(m_currentStr);
        appendToken();
    }
}
