#include "Token.hpp"

namespace tl::fe {
  static const std::unordered_map<std::string, EToken> nonTypeKeywordTable = {
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

  static const std::unordered_set<std::string> fundamentalTypes{
    "int", "float", "bool", "char", "string"
  };

  static const std::unordered_set<EToken> reservedTokens{
  };

  static const std::unordered_map<std::string, EToken> operatorTable{
    {">>=", EToken::Greater2Equal},
    {"<<=", EToken::Less2Equal},
    {"...", EToken::Dot3},
    {"!=", EToken::ExclaimEqual},
    {"*=", EToken::StarEqual},
    {"&=", EToken::AmpersandEqual},
    {"|=", EToken::BarEqual},
    {"^=", EToken::HatEqual},
    {"/=", EToken::FwdSlashEqual},
    {"%=", EToken::PercentEqual},
    {">=", EToken::GreaterEqual},
    {"<=", EToken::LessEqual},
    {"+=", EToken::PlusEqual},
    {"-=", EToken::MinusEqual},
    {"|>", EToken::BarGreater},
    {"->", EToken::MinusGreater},
    {"<-", EToken::LessMinus},
    {"::", EToken::Colon2},
    {"**", EToken::Star2},
    {"&&", EToken::Ampersand2},
    {"||", EToken::Bar2},
    {"++", EToken::Plus2},
    {"--", EToken::Minus2},
    {"//", EToken::FwdSlash2},
    {"!!", EToken::Exclaim2},
    {"==", EToken::Equal2},
    {">>", EToken::Greater2},
    {"<<", EToken::Less2},
    {"??", EToken::QMark2},
    {"..", EToken::Dot2},
    {"(", EToken::LeftParen},
    {")", EToken::RightParen},
    {"[", EToken::LeftBracket},
    {"]", EToken::RightBracket},
    {"{", EToken::LeftBrace},
    {"}", EToken::RightBrace},
    {".", EToken::Dot},
    {",", EToken::Comma},
    {":", EToken::Colon},
    {";", EToken::Semicolon},
    {"*", EToken::Star},
    {"&", EToken::Ampersand},
    {"|", EToken::Bar},
    {"^", EToken::Hat},
    {"+", EToken::Plus},
    {"-", EToken::Minus},
    {"/", EToken::FwdSlash},
    {"%", EToken::Percent},
    {"!", EToken::Exclaim},
    {"=", EToken::Equal},
    {">", EToken::Greater},
    {"<", EToken::Less},
    {"'", EToken::SQuote},
    {"\"", EToken::DQuote},
    {"?", EToken::QMark},
    {"~", EToken::Tilde},
    {"$", EToken::Dollar},
  };

  static auto identifierType(const std::string &idStr) noexcept -> EToken {
    if (
      const auto it = nonTypeKeywordTable.find(idStr);
      it != nonTypeKeywordTable.end()
    ) {
      return it->second;
    }

    if (
      const auto it = fundamentalTypes.find(idStr);
      it != fundamentalTypes.end()
    ) {
      return EToken::FundamentalType;
    }

    return EToken::Identifier;
  }

  static auto operatorType(const std::string &opStr) noexcept -> EToken {
    const auto it = operatorTable.find(opStr);
    return it == operatorTable.end() ? EToken::Invalid : it->second;
  }

  Token::Token(EToken type, std::string str, sz line, sz column)
    : m_type(identifierType(str)), m_str(std::move(str)), m_line(line), m_column(column) {
    if (m_type == EToken::Identifier) {
      m_type = identifierType(m_str);
    }
    if (m_type == EToken::MaybeOperator) {
      m_type = operatorType(m_str);
    }

    const auto it = reservedTokens.find(m_type);
    if (it == reservedTokens.end()) {
      m_type = EToken::Reserved;
    }
  }
}
