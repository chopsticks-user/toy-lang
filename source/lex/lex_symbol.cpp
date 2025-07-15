#include "lex.hpp"
#include "util.hpp"

namespace tlc::lex {
    using OpGraph3 = HashMap<char, HashMap<char, HashSet<char>>>;

    static const HashMap<StrV, token::EToken> operatorTable{
        {":=>", token::EToken::ColonEqualGreater},
        {":~>", token::EToken::ColonTildeGreater},
        {">>=", token::EToken::Greater2Equal},
        {"<<=", token::EToken::Less2Equal},
        {"**=", token::EToken::Star2Equal},
        {"...", token::EToken::Dot3},
        {"!=", token::EToken::ExclaimEqual},
        {"*=", token::EToken::StarEqual},
        {"&=", token::EToken::AmpersandEqual},
        {"|=", token::EToken::BarEqual},
        {"^=", token::EToken::HatEqual},
        {"/=", token::EToken::FwdSlashEqual},
        {"%=", token::EToken::PercentEqual},
        {">=", token::EToken::GreaterEqual},
        {"<=", token::EToken::LessEqual},
        {"+=", token::EToken::PlusEqual},
        {"-=", token::EToken::MinusEqual},
        {"|>", token::EToken::BarGreater},
        {"->", token::EToken::MinusGreater},
        {"=>", token::EToken::EqualGreater},
        // {"<-", token::EToken::LessMinus},
        {"::", token::EToken::Colon2},
        {"**", token::EToken::Star2},
        {"&&", token::EToken::Ampersand2},
        {"||", token::EToken::Bar2},
        {"++", token::EToken::Plus2},
        {"--", token::EToken::Minus2},
        // {"!!", token::EToken::Exclaim2},
        {"==", token::EToken::Equal2},
        {">>", token::EToken::Greater2},
        {"<<", token::EToken::Less2},
        {"??", token::EToken::QMark2},
        {"..", token::EToken::Dot2},
        {"(", token::EToken::LeftParen},
        {")", token::EToken::RightParen},
        {"[", token::EToken::LeftBracket},
        {"]", token::EToken::RightBracket},
        {"{", token::EToken::LeftBrace},
        {"}", token::EToken::RightBrace},
        {".", token::EToken::Dot},
        {",", token::EToken::Comma},
        {":", token::EToken::Colon},
        {";", token::EToken::Semicolon},
        {"*", token::EToken::Star},
        {"&", token::EToken::Ampersand},
        {"#", token::EToken::Hash},
        {"|", token::EToken::Bar},
        {"^", token::EToken::Hat},
        {"+", token::EToken::Plus},
        {"-", token::EToken::Minus},
        {"/", token::EToken::FwdSlash},
        {"%", token::EToken::Percent},
        {"!", token::EToken::Exclaim},
        {"=", token::EToken::Equal},
        {">", token::EToken::Greater},
        {"<", token::EToken::Less},
        {"'", token::EToken::SQuote},
        {"\"", token::EToken::DQuote},
        {"?", token::EToken::QMark},
        {"~", token::EToken::Tilde},
        {"$", token::EToken::Dollar},
        {"@", token::EToken::At},
    };

    static const OpGraph3 opGraph = [] {
        OpGraph3 graph;
        rng::for_each(
            operatorTable
            | rv::transform([](auto const& entry) {
                return entry.first;
            }), [&graph](StrV const op) {
                if (!graph.contains(op[0])) {
                    graph[op[0]] = {};
                }
                if (op.length() > 1) {
                    if (!graph[op[0]].contains(op[1])) {
                        graph[op[0]][op[1]] = {};
                    }
                    if (op.length() > 2) {
                        graph[op[0]][op[1]].emplace(op[2]);
                    }
                }
            });
        return graph;
    }();

    auto Lexer::lexSymbol() -> void {
        if (isUnnamedIdentifier(m_stream.current())) {
            m_currentTokenType = token::EToken::AnonymousIdentifier;
            appendToken();
            return;
        }

        if (auto const entry = opGraph.find(m_stream.current());
            entry != opGraph.end()) {
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
        m_currentTokenType = operatorTable.at(m_currentLexeme);
        appendToken();
    }
}
