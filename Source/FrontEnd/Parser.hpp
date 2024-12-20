#ifndef  TOYLANG_FRONTEND_PARSER_HPP
#define  TOYLANG_FRONTEND_PARSER_HPP

#include "Syntax/Syntax.hpp"
#include "Lexer.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  struct ParserExpception final : ToyLangException {
    ParserExpception(CRef<String> filepath, CRef<String> mesg)
      : ToyLangException{filepath, mesg} {
    }

    ParserExpception(
      CRef<String> filepath, CRef<Token> token, CRef<String> mesg
    ): ToyLangException{filepath, token.line(), token.column(), "syntax error: " + mesg} {
    }
  };

  class Parser {
    using TokenIterator = Tokens::const_iterator;

  public:
    auto operator()(
      ExpceptionCollector &eCollector, String filepath, Tokens tokens
    ) -> syntax::TranslationUnit;

  private:
    auto revert() -> void;

    auto advance() -> void;

    auto current() const -> CRef<Token>;

    auto match(std::same_as<EToken> auto... expected) -> bool;

    auto peekNext() -> Token;

    auto peekPrev() -> Token;

    auto createError(ToyLangException &&e) const -> void;

  private:
    auto parseTranslationUnit() -> syntax::TranslationUnit;

    auto parseModuleDecl() -> syntax::ASTNode;

    auto parseImportDecl() -> syntax::ASTNode;

    auto parseTypeDecl() -> syntax::ASTNode;

    auto parseFunctionDef() -> syntax::ASTNode;

    auto parseClassDef() -> syntax::ASTNode;

    auto parseInterfaceDef() -> syntax::ASTNode;

    auto parseIdentifier() -> syntax::ASTNode;

  private:
    String m_filepath;
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
    const Token *m_currentToken = nullptr;

    bool classState = false;
    bool interfaceState = false;
    syntax::Storage currentStorage = syntax::Storage::Internal;
    syntax::Access currentAccess = syntax::Access::Private;

    ExpceptionCollector *m_eCollector = nullptr;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
