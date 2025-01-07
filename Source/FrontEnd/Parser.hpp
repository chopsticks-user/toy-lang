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

    enum class TupleDeclState {
      Local, Param, Return,
    };

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

    auto collectException(ToyLangException &&e) const -> void;

    auto collectException(CRef<String> mesg) const -> void;

    auto setCurrentStorage(syntax::Storage storage = syntax::Storage::Internal) -> void;

    auto currentStorage() const noexcept -> syntax::Storage;

    auto markRevertPoint() noexcept -> void;

    auto toRevertPoint() noexcept -> void;

  private:
    auto parseTranslationUnit() -> syntax::TranslationUnit;

    auto parseModuleDecl() -> syntax::ASTNode;

    auto parseImportDecl() -> syntax::ASTNode;

    auto parseTypeDecl() -> syntax::ASTNode;

    auto parseFunctionDef() -> syntax::ASTNode;

    auto parseClassDef() -> syntax::ASTNode;

    auto parseConceptDef() -> syntax::ASTNode;

    auto parseIdentifier() -> syntax::ASTNode;

    auto parseTypeIdentifier() -> syntax::ASTNode;

    auto parseTypeExpr() -> syntax::ASTNode;

    auto parseFunctionPrototype() -> syntax::ASTNode;

    auto parseTupleDecl() -> syntax::ASTNode;

    auto parseParameterDecl() -> syntax::ASTNode;

    auto parseReturnDecl() -> syntax::ASTNode;

    auto parseIdentifierDecl() -> syntax::ASTNode;

    // auto parseExpr() -> syntax::ASTNode;

  private:
    String m_filepath;
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
    Opt<Token> m_currentToken;
    Opt<TokenIterator> m_revertPoint;
    syntax::Storage m_currentStorage = syntax::Storage::Internal;
    ExpceptionCollector *m_eCollector = nullptr;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
