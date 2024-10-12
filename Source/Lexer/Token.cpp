#include "Token.hpp"

namespace tl::lexer {
  static const std::unordered_map<std::string, EToken> reservedKeywordTable = {
    {"fn", EToken::Fn},
    {"main", EToken::Main},
    {"class", EToken::Class},
    {"super", EToken::Super},
    {"self", EToken::Self},
    {"public", EToken::Public},
    {"private", EToken::Private},
    {"protected", EToken::Protected},
    {"return", EToken::Return},
    {"for", EToken::For},
    {"while", EToken::While},
    {"if", EToken::If},
    {"switch", EToken::Switch},
    {"case", EToken::Case},
    {"default", EToken::Default},
    {"export", EToken::Export},
    {"import", EToken::Import},
    {"module", EToken::Module},
    {"internal", EToken::Internal},
    {"local", EToken::Local},
    {"pure", EToken::Pure},
    {"var", EToken::Var},
    {"const", EToken::Const},
    {"print", EToken::Print},
    {"extern", EToken::Extern},
    {"abstract", EToken::Abstract},
    {"interface", EToken::Interface},
  };

  auto Token::identifierType(const std::string &idStr) noexcept -> EToken {
    const auto it = reservedKeywordTable.find(idStr);
    return it != reservedKeywordTable.end() ? it->second : EToken::Identifier;
  }
}
