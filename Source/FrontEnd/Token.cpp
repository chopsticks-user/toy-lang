#include "Token.hpp"

namespace tl::fe {
  static const HashMap<StringView, EToken> nonTypeKeywordTable = {
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
    {"by", EToken::By},
    {"else", EToken::Else},
    {"let", EToken::Let},
    {"mutable", EToken::Mutable},
  };

  static const HashSet<StringView> fundamentalTypes{
    "Int", "Float", "Bool", "Char", "Void",
    "Array", "Set", "Tuple", "Fn", "String"
  };

  static const HashSet<StringView> reservedKeywords{
    "int", "float", "bool", "char", "void",
    "array", "set", "tuple", "fn", "string"
  };

  static const HashMap<StringView, EToken> operatorTable{
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

  static const auto identifierType(const StringView idStr) noexcept -> EToken {
    if (idStr.front() >= 'A' && idStr.front() <= 'Z') {
      if (const auto it = fundamentalTypes.find(idStr); it != fundamentalTypes.end()) {
        return EToken::FundamentalType;
      }

      return EToken::UserDefinedType;
    }

    if (reservedKeywords.contains(idStr)) {
      return EToken::Reserved;
    }

    if (const auto it = nonTypeKeywordTable.find(idStr); it != nonTypeKeywordTable.end()) {
      return it->second;
    }

    return EToken::Identifier;
  }

  static const auto operatorType(const StringView opStr) noexcept -> EToken {
    if (const auto it = operatorTable.find(opStr); it != operatorTable.end()) {
      return it->second;
    }
    return EToken::Invalid;
  }

  Token::Token(const EToken type, String str, const sz line, const sz column)
    : m_type(type), m_str(std::move(str)), m_line(line), m_column(column) {
    if (m_type == EToken::Identifier) {
      m_type = identifierType(m_str);
    }
    if (m_type == EToken::MaybeOperator) {
      m_type = operatorType(m_str);
    }
  }

  auto Token::isValidOperator(const StringView symbol) -> bool {
    return operatorType(symbol) != EToken::Invalid;
  }
}
