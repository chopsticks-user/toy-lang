#ifndef TLC_PARSE_STREAM_HPP
#define TLC_PARSE_STREAM_HPP

#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::parse {
    class TokenStream final {
    public:
        using MatchFn = bool (*)(lexeme::Lexeme const&);
        using TokenIt = token::TokenizedBuffer::const_iterator;

        class Backtrack final {
        public:
            explicit Backtrack(TokenStream& stream)
                : m_stream{stream} {
                m_stream.markBacktrack();
            }

            auto operator()() -> void {
                if (m_executed) {
                    return;
                }
                m_stream.backtrack();
                m_executed = true;
            }

            ~Backtrack() noexcept {
                if (!m_executed) {
                    m_stream.removeBacktrack();
                }
            }

        private:
            TokenStream& m_stream;
            bool m_executed = false;
        };

    public:
        explicit TokenStream(token::TokenizedBuffer tokens, Opt<Location> offset = {})
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

        auto match(std::same_as<lexeme::Lexeme> auto... types) -> bool {
            auto const tokenType = peek().lexeme();
            if (done() || ((tokenType != types) && ...)) {
                return false;
            }
            advance();
            return true;
        }

        auto match(MatchFn cond) -> bool;

        auto advance() -> void;

        [[nodiscard]] auto peek() const -> token::Token;

        auto markBacktrack() -> void {
            m_backtrack.push({m_tokenIt, m_started});
        }

        // todo: implement scoped backtrack
        auto removeBacktrack() noexcept -> void {
            if (m_backtrack.empty()) {
                return;
            }
            m_backtrack.pop();
        }

        auto backtrack() -> void;

        auto scopedBacktrack() -> Backtrack {
            return Backtrack{*this};
        }

        [[nodiscard]] auto current() const -> token::Token;

        [[nodiscard]] auto done() const -> b8 {
            // todo:
            return m_started && m_tokenIt == m_tokens.end();
        }

    private:
        static auto makeInvalidToken() -> token::Token {
            return {lexeme::invalid, "", {0, 0}};
        }

    private:
        struct BacktrackStates {
            TokenIt tokenIt;
            b8 started;
        };

        token::TokenizedBuffer const m_tokens;
        TokenIt m_tokenIt;
        Stack<BacktrackStates> m_backtrack{};
        b8 m_started = false;
    };
}

#endif // TLC_PARSE_STREAM_HPP
