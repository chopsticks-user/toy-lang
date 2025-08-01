#include "token_stream.hpp"

namespace tlc::parse {
    auto TokenStream::match(MatchFn const cond) -> bool {
        if (done() || !cond(peek().lexeme())) {
            return false;
        }
        advance();
        return true;
    }

    auto TokenStream::advance() -> void {
        if (!m_started) {
            m_started = true;
            return;
        }
        ++m_tokenIt;
    }

    auto TokenStream::peek() const -> token::Token {
        if (!m_started) {
            return *m_tokenIt;
        }
        if (auto const nextIt = std::next(m_tokenIt);
            nextIt != m_tokens.end()) {
            return *nextIt;
        }
        return makeInvalidToken();
    }

    auto TokenStream::backtrack() -> void {
        if (m_backtrack.empty()) {
            return;
        }
        auto [it, started] = m_backtrack.top();
        m_tokenIt = it;
        m_started = started;
        m_backtrack.pop();
    }

    auto TokenStream::current() const -> token::Token {
        if (done() || !m_started) {
            return makeInvalidToken();
        }
        return *m_tokenIt;
    }


}
