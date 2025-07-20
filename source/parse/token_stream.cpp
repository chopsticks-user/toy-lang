#include "token_stream.hpp"

namespace tlc::parse {
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

    auto TokenStream::unmarkAndReturnCoords()
    -> token::Token::Coords {
        if (m_markedTokens.empty()) {
            return makeInvalidToken().coords();
        }

        auto const coords = m_markedTokens.top().coords();
        m_markedTokens.pop();
        return coords;
    }

    auto TokenStream::current() const -> token::Token {
        if (done() || !m_started) {
            return makeInvalidToken();
        }
        return *m_tokenIt;
    }
}
