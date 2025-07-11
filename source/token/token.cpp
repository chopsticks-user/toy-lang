#include "token.hpp"

namespace tlc::token {
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
