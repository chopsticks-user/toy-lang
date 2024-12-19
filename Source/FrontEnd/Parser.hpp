#ifndef  TOYLANG_FRONTEND_PARSER_HPP
#define  TOYLANG_FRONTEND_PARSER_HPP

#include "Syntax/Syntax.hpp"
#include "Lexer.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class Parser {
    using TokenIterator = Tokens::const_iterator;

  public:
    auto operator()(Tokens tokens) -> syntax::TranslationUnit;

  private:
    auto revert() -> void;

    auto advance() -> void;

    auto current() const -> Token;

    auto match(std::same_as<EToken> auto... expected) -> bool;

    auto peekNext() -> Token;

    auto peekPrev() -> Token;

  private:
    auto parseTranslationUnit() -> syntax::TranslationUnit;

  private:
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
