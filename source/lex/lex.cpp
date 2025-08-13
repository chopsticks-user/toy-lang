export module lex;

import :util;
import :text_stream;
import token;
import lexeme;
#include "core/core.hpp"

namespace tlc::lex {
    export class Lex final {
    public:
        static constexpr auto operator()(fs::path const& filepath)
            -> token::TokenizedBuffer {
            return Lex{filepath}();
        }

        static constexpr auto operator()(std::istringstream iss)
            -> token::TokenizedBuffer {
            return Lex{std::move(iss)}();
        }

        explicit constexpr Lex(fs::path const& filepath)
            : m_stream{filepath} {}

        explicit constexpr Lex(std::istringstream iss)
            : m_stream{std::move(iss)} {}

        constexpr auto operator()() -> token::TokenizedBuffer {
            m_stream.consumeSpaces();
            while (!m_stream.done()) {
                m_currentStr = "";
                m_currentLexeme = lexeme::invalid;

                if (m_stream.match(isCommentOuter)) {
                    markTokenLocation();
                    appendStr();
                    lexComment();
                }
                else if (m_stream.match(isDigit)) {
                    markTokenLocation();
                    appendStr();
                    lexNumeric();
                }
                else if (m_stream.match(isLetter)) {
                    markTokenLocation();
                    appendStr();
                    lexIdentifier();
                }
                else if (m_stream.match(isStringTerminator)) {
                    markTokenLocation();
                    lexString();
                }
                else {
                    m_stream.advance();
                    markTokenLocation();
                    lexSymbol();
                }

                m_stream.consumeSpaces();
            }

            return m_tokens;
        }

    private:
        constexpr auto lexComment() -> void {}

        constexpr auto lexIdentifier() -> void {
            while (m_stream.match(isDigitOrLetter)) {
                appendStr();
            }
            classifyIdentifier(m_currentStr);
            appendToken();
        }

        constexpr auto lexFloatingPoint() -> void {
            if (m_stream.match(isStartOfDecimalPart)) {
                appendStr();
                m_currentLexeme = lexeme::floatLiteral;
                szt count = 0;
                while (m_stream.match(isDigit)) {
                    appendStr();
                    ++count;
                }
                if (count == 0) {
                    // todo: throw
                    m_currentLexeme = lexeme::invalid;
                }
            }
        }

        constexpr auto lexNondecimalNumeric() -> void {
            szt count = 0;
            if (m_stream.match('x')) {
                appendStr();
                m_currentLexeme = lexeme::integer16Literal;

                while (m_stream.match(isHexadecimalDigit)) {
                    appendStr();
                    ++count;
                }
            }
            else if (m_stream.match('b')) {
                appendStr();
                m_currentLexeme = lexeme::integer2Literal;

                while (m_stream.match(isBinaryDigit)) {
                    appendStr();
                    ++count;
                }
            }
            else {
                m_currentLexeme = lexeme::integer8Literal;

                while (m_stream.match(isOctalDigit)) {
                    appendStr();
                    ++count;
                }
                lexFloatingPoint();

                if (m_currentStr.length() > count + 1) {
                    m_currentLexeme = lexeme::floatLiteral;
                }
            }

            if (count == 0 && m_currentLexeme != lexeme::floatLiteral) {
                // simply "0"
                m_currentLexeme = lexeme::integer10Literal;
            }

            appendToken();
        }

        constexpr auto lexNumeric() -> void {
            m_currentLexeme = lexeme::integer10Literal;
            if (m_stream.current() == '0') {
                return lexNondecimalNumeric();
            }

            while (m_stream.match(isDigit)) {
                appendStr();
            }
            lexFloatingPoint();

            appendToken();
        }

        constexpr auto lexSymbol() -> void {
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

        constexpr auto lexString() -> void {
            m_currentLexeme = lexeme::stringFragment;

            b8 escaped = false;
            b8 lastTokenIsPlaceholder = false;
            const auto toFragmentState = [&] {
                markTokenLocation();
                m_currentStr = "";
                m_currentLexeme = lexeme::stringFragment;
                lastTokenIsPlaceholder = false;
            };

            while (!m_stream.match(isStringTerminator) && m_stream.peek() != '\n') {
                m_stream.advance();
                if (lastTokenIsPlaceholder) {
                    toFragmentState();
                }
                if (auto const c = m_stream.current(); escaped) {
                    switch (c) {
                    case 'n': {
                        appendStr('\n');
                        break;
                    }
                    case 't': {
                        appendStr('\t');
                        break;
                    }
                    case 'r': {
                        appendStr('\r');
                        break;
                    }
                    case 'b': {
                        appendStr('\b');
                        break;
                    }
                    case 'f': {
                        appendStr('\f');
                        break;
                    }
                    case 'a': {
                        appendStr('\a');
                        break;
                    }
                    case '\\': {
                        appendStr('\\');
                        break;
                    }
                    case '\'': {
                        appendStr('\'');
                        break;
                    }
                    case '"': {
                        appendStr('"');
                        break;
                    }
                    case '{': {
                        appendStr('{');
                        break;
                    }
                    case '}': {
                        appendStr('}');
                        break;
                    }
                    default: {
                        appendStr('\\');
                        appendStr(c);
                        break;
                    }
                    }
                    escaped = false;
                }
                else if (c == '\\') {
                    escaped = true;
                }
                else if (c == '{') {
                    // consume string until the correct enclosing character '}' is found.
                    // the consumed string will be "re-lexed" from a parser
                    appendToken();
                    markTokenLocation();
                    m_currentStr = "";
                    m_currentLexeme = lexeme::stringPlaceholder;

                    szt left = 1, right = 0;
                    c8 last = m_stream.current();
                    while (left != right) {
                        m_stream.advance();

                        auto const cc = m_stream.current();
                        if (cc == '{' && last != '\\') {
                            ++left;
                        }
                        else if (cc == '}' && last != '\\') {
                            ++right;
                        }
                        else {
                            appendStr();
                        }
                        last = cc;
                    }

                    if (left != right) {
                        // todo: error
                    }
                    appendToken();
                    lastTokenIsPlaceholder = true;
                }
                else {
                    appendStr();
                }
            }

            if (lastTokenIsPlaceholder) {
                toFragmentState();
            }

            if (!isStringTerminator(m_stream.current())) {
                // todo: error
            }

            appendToken();
        }

    private:
        constexpr auto classifyIdentifier(StrV const lexeme) -> void {
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

        constexpr auto markTokenLocation() -> void {
            m_tokenLine = m_stream.line();
            m_tokenColumn = m_stream.column();
        }

        constexpr auto appendStr() -> void {
            m_currentStr += m_stream.current();
        }

        constexpr auto appendStr(c8 const c) -> void {
            m_currentStr += c;
        }

        constexpr auto appendToken() -> void {
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
