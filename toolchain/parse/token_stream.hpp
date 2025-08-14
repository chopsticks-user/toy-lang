#ifndef TLC_PARSE_STREAM_HPP
#define TLC_PARSE_STREAM_HPP

#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::parse {
    // todo: 'invalid' should be replaced. Insert an empty token at the end
    class TokenStream final {
    public:
        using MatchFn = bool (*)(lexeme::Lexeme const&);
        using TokenIt = token::TokenizedBuffer::const_iterator;

    public:
        explicit constexpr TokenStream(token::TokenizedBuffer tokens, Opt<Location> offset = {})
            : m_tokens{
                  offset
                      ? tokens | rv::transform([offset](token::Token const& token) {
                          return token::Token{
                              token.lexeme(), token.str(), Location{
                                  .line = token.line() + offset->line,
                                  .column = token.column() + offset->column,
                              }
                          };
                      }) | rng::to<token::TokenizedBuffer>()
                      : std::move(tokens)
              },
              m_tokenIt{m_tokens.begin()} {}

        constexpr auto match(std::same_as<lexeme::Lexeme> auto... types) -> bool {
            auto const tokenType = peek().lexeme();
            if (itDone() || ((tokenType != types) && ...)) {
                return false;
            }
            advance();
            return true;
        }

        constexpr auto match(MatchFn const cond) -> bool {
            if (itDone() || !cond(peek().lexeme())) {
                return false;
            }
            advance();
            return true;
        }

        constexpr auto advance() -> void {
            if (!m_started) {
                m_started = true;
                return;
            }
            ++m_tokenIt;
        }

        [[nodiscard]] constexpr auto peek() const -> token::Token {
            if (!m_started) {
                return *m_tokenIt;
            }
            if (auto const nextIt = std::next(m_tokenIt);
                nextIt != m_tokens.end()) {
                return *nextIt;
            }
            return makeInvalidToken();
        }

        constexpr auto markBacktrack() -> void {
            m_backtrack.push({m_tokenIt, m_started});
        }

        constexpr auto removeBacktrack() noexcept -> void {
            if (m_backtrack.empty()) {
                return;
            }
            m_backtrack.pop();
        }

        constexpr auto backtrack() -> void {
            if (m_backtrack.empty()) {
                return;
            }
            auto [it, started] = m_backtrack.top();
            m_tokenIt = it;
            m_started = started;
            m_backtrack.pop();
        }

        [[nodiscard]] constexpr auto current() const -> token::Token {
            if (itDone() || !m_started) {
                return makeInvalidToken();
            }
            return *m_tokenIt;
        }

        [[nodiscard]] constexpr auto done() const -> b8 {
            // todo:
            return m_started && peek().lexeme() == lexeme::invalid;
        }

    private:
        static constexpr auto makeInvalidToken() -> token::Token {
            return {lexeme::invalid, "", {0, 0}};
        }

        constexpr auto itDone() const -> b8 {
            return m_started && m_tokenIt == m_tokens.end();
        }

        struct BacktrackStates {
            TokenIt tokenIt;
            b8 started;
        };

    private:
        token::TokenizedBuffer const m_tokens;
        TokenIt m_tokenIt;
        Stack<BacktrackStates> m_backtrack{};
        b8 m_started = false;
    };
}

#endif // TLC_PARSE_STREAM_HPP
