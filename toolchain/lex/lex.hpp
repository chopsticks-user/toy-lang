#ifndef TLC_LEX_HPP
#define TLC_LEX_HPP

#include "core/core.hpp"
#include "token/token.hpp"

#include "text_stream.hpp"

namespace tlc::lex {
    class Lex final {
    public:
        static auto operator()(fs::path const& filepath) -> token::TokenizedBuffer;
        static auto operator()(std::istringstream iss) -> token::TokenizedBuffer;

        explicit Lex(fs::path const& filepath)
            : m_stream{filepath} {}

        explicit Lex(std::istringstream iss)
            : m_stream{std::move(iss)} {}

        auto operator()() -> token::TokenizedBuffer;

    private:
        auto lexComment() -> void;
        auto lexIdentifier() -> void;
        auto lexFloatingPoint() -> void;
        auto lexNondecimalNumeric() -> void;
        auto lexNumeric() -> void;
        auto lexSymbol() -> void;
        auto lexString() -> void;

    private:
        auto classifyIdentifier(StrV lexeme) -> void;

        auto markTokenLocation() -> void {
            m_tokenLine = m_stream.line();
            m_tokenColumn = m_stream.column();
        }

        auto appendStr() -> void {
            m_currentStr += m_stream.current();
        }

        auto appendStr(c8 const c) -> void {
            m_currentStr += c;
        }

        auto appendToken() -> void {
            if (m_currentLexeme == lexeme::invalid) {
                // todo: throw
                return;
            }

            m_tokens.emplace_back(
                m_currentLexeme, m_currentStr,
                Location{m_tokenLine, m_tokenColumn}
            );
        }

    private:
        TextStream m_stream;
        lexeme::Lexeme m_currentLexeme = lexeme::invalid;
        Str m_currentStr{};
        szt m_tokenLine{}, m_tokenColumn{};
        token::TokenizedBuffer m_tokens{};
    };
}

#endif // TLC_LEX_HPP
