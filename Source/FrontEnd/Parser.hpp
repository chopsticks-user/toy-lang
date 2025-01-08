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

    enum class Context {
      None, Global, Function, Concept, Class, Type, String,
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

    auto setStorage(syntax::Storage storage = syntax::Storage::Internal) -> void;

    auto storage() const noexcept -> syntax::Storage;

    auto setContext(Context context = Context::None) noexcept -> void;

    auto enterContext(Context context = Context::None) noexcept -> void;

    auto exitContext() noexcept -> void;

    auto context() const noexcept -> Context;

    auto markRevertPoint() noexcept -> void;

    auto toRevertPoint() noexcept -> void;

  protected:
    auto parseTranslationUnit() -> syntax::TranslationUnit;

    auto parseModuleDecl() -> syntax::ASTNode;

    auto parseImportDecl() -> syntax::ASTNode;

    auto parseTypeDecl() -> syntax::ASTNode;

    auto parseFunctionDef() -> syntax::ASTNode;

    auto parseClassDef() -> syntax::ASTNode;

    auto parseConceptDef() -> syntax::ASTNode;

    auto parseFunctionPrototype() -> syntax::ASTNode;

    auto parseTupleDecl() -> syntax::ASTNode;

    auto parseIdentifierDecl() -> syntax::ASTNode;

    auto parseTypeExpr() -> syntax::ASTNode;

    auto parseTypeIdentifier() -> syntax::ASTNode;

    auto parseIdentifier() -> syntax::ASTNode;

    auto parseStmt() -> syntax::ASTNode;

    auto parseForStmt() -> syntax::ASTNode;

    auto parseMatchStmt() -> syntax::ASTNode;

    auto parseBlockStmt() -> syntax::ASTNode;

    auto parseAssignOrExprStmt() -> syntax::ASTNode;

    auto parseLetStmt() -> syntax::ASTNode;

    auto parseReturnStmt() -> syntax::ASTNode;

    auto parseExpr() -> syntax::ASTNode;

    auto parseTupleExpr() -> syntax::ASTNode;

  private:
    String m_filepath;
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
    Opt<Token> m_currentToken;
    Opt<TokenIterator> m_revertPoint;
    syntax::Storage m_currentStorage = syntax::Storage::Internal;
    Stack<Context> m_parseContext;
    ExpceptionCollector *m_eCollector = nullptr; // todo: global singleton
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
