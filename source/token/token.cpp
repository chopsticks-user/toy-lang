#include "token.hpp"

namespace tlc::token {
    // static const HashMap<StrV, EToken> operatorTable{
    //   {":=>", EToken::ColonEqualGreater},
    //   {":~>", EToken::ColonTildeGreater},
    //   {">>=", EToken::Greater2Equal},
    //   {"<<=", EToken::Less2Equal},
    //   {"**=", EToken::Star2Equal},
    //   {"...", EToken::Dot3},
    //   {"!=", EToken::ExclaimEqual},
    //   {"*=", EToken::StarEqual},
    //   {"&=", EToken::AmpersandEqual},
    //   {"|=", EToken::BarEqual},
    //   {"^=", EToken::HatEqual},
    //   {"/=", EToken::FwdSlashEqual},
    //   {"%=", EToken::PercentEqual},
    //   {">=", EToken::GreaterEqual},
    //   {"<=", EToken::LessEqual},
    //   {"+=", EToken::PlusEqual},
    //   {"-=", EToken::MinusEqual},
    //   {"|>", EToken::BarGreater},
    //   {"->", EToken::MinusGreater},
    //   {"=>", EToken::EqualGreater},
    //   // {"<-", EToken::LessMinus},
    //   {"::", EToken::Colon2},
    //   {"**", EToken::Star2},
    //   {"&&", EToken::Ampersand2},
    //   {"||", EToken::Bar2},
    //   {"++", EToken::Plus2},
    //   {"--", EToken::Minus2},
    //   // {"!!", EToken::Exclaim2},
    //   {"==", EToken::Equal2},
    //   {">>", EToken::Greater2},
    //   {"<<", EToken::Less2},
    //   {"??", EToken::QMark2},
    //   {"..", EToken::Dot2},
    //   {"(", EToken::LeftParen},
    //   {")", EToken::RightParen},
    //   {"[", EToken::LeftBracket},
    //   {"]", EToken::RightBracket},
    //   {"{", EToken::LeftBrace},
    //   {"}", EToken::RightBrace},
    //   {".", EToken::Dot},
    //   {",", EToken::Comma},
    //   {":", EToken::Colon},
    //   {";", EToken::Semicolon},
    //   {"*", EToken::Star},
    //   {"&", EToken::Ampersand},
    //   {"#", EToken::Hash},
    //   {"|", EToken::Bar},
    //   {"^", EToken::Hat},
    //   {"+", EToken::Plus},
    //   {"-", EToken::Minus},
    //   {"/", EToken::FwdSlash},
    //   {"%", EToken::Percent},
    //   {"!", EToken::Exclaim},
    //   {"=", EToken::Equal},
    //   {">", EToken::Greater},
    //   {"<", EToken::Less},
    //   {"'", EToken::SQuote},
    //   {"\"", EToken::DQuote},
    //   {"?", EToken::QMark},
    //   {"~", EToken::Tilde},
    //   {"$", EToken::Dollar},
    //   {"@", EToken::At},
    // };
    //
    // static auto operatorType(const StrV opStr) noexcept -> EToken {
    //   if (const auto it = operatorTable.find(opStr); it != operatorTable.end()) {
    //     return it->second;
    //   }
    //   return EToken::Invalid;
    // }
    //
    // Token::Token(const EToken type, Str str, const szt line, const szt column)
    //   : m_type(type), m_str(std::move(str)), m_line(line), m_column(column) {
    //   if (m_type == EToken::Identifier) {
    //     m_type = identifierType(m_str);
    //   }
    //   if (m_type == EToken::MaybeOperator) {
    //     m_type = operatorType(m_str);
    //   }
    // }
    //
    // auto Token::isValidOperator(const StrV symbol) -> bool {
    //   return operatorType(symbol) != EToken::Invalid;
    // }
}
