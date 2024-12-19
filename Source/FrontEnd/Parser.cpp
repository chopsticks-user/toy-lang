#include "Parser.hpp"

using tl::syntax::ASTNode;

namespace tl::fe {
  auto Parser::current() const -> Token {
    return *m_tokenIt;
  }

  auto Parser::revert() -> void {
    --m_tokenIt;
  }

  auto Parser::advance() -> void {
    ++m_tokenIt;
  }

  auto Parser::match(std::same_as<EToken> auto... expected) -> bool {
    if (!((m_tokenIt->type() == expected) || ...)) {
      return false;
    }

    ++m_tokenIt;
    return true;
  }

  auto Parser::peekNext() -> Token {
    ++m_tokenIt;
    return *(m_tokenIt--);
  }

  auto Parser::peekPrev() -> Token {
    --m_tokenIt;
    return *(m_tokenIt++);
  }

  auto Parser::operator()(Tokens tokens) -> syntax::TranslationUnit {
    m_tokenIt = tokens.begin();
    m_tokenItEnd = tokens.end();
    return parseTranslationUnit();
  }

  auto Parser::parseTranslationUnit() -> syntax::TranslationUnit {
  }
}
