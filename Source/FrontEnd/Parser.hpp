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

    auto parseClassDefinition() -> syntax::ASTNode;

    auto parseFunctionDefinition() -> syntax::ASTNode;

    auto parseFunctionPrototype() -> syntax::ASTNode;

    auto parseIdentifierDeclStatement() -> syntax::ASTNode;

    auto parseIdentifierDeclFragment() -> syntax::ASTNode;

    auto parseModuleStatement() -> syntax::ASTNode;

    auto parseImportStatement() -> syntax::ASTNode;

    auto parseSpecifier() -> std::string;

    auto parseVisibilitySpecifier() -> std::string;

    auto parseExpression() -> syntax::ASTNode;

    auto parseSequenceExpression() -> syntax::ASTNode;

    auto parseTernaryExpression() -> syntax::ASTNode;

    auto parseNullCoalescingExpression() -> syntax::ASTNode;

    auto parseLogicalOrExpression() -> syntax::ASTNode;

    auto parseLogicalAndExpression() -> syntax::ASTNode;

    auto parseInclusiveOrExpression() -> syntax::ASTNode;

    auto parseExclusiveOrExpression() -> syntax::ASTNode;

    auto parseAndExpression() -> syntax::ASTNode;

    auto parseEqualityExpression() -> syntax::ASTNode;

    auto parseRelationalExpression() -> syntax::ASTNode;

    auto parseShiftExpression() -> syntax::ASTNode;

    auto parseAdditiveExpression() -> syntax::ASTNode;

    auto parseMultiplicativeExpression() -> syntax::ASTNode;

    auto parsePrefixUnaryExpression() -> syntax::ASTNode;

    auto parsePostfixExpression() -> syntax::ASTNode;

    auto parsePrimaryExpression() -> syntax::ASTNode;

    auto parseArgumentList() -> std::vector<syntax::ASTNode>;

    auto parseBlockStatement() -> syntax::ASTNode;

    auto parseTypeExpression() -> syntax::ASTNode;

    auto parseParameterDeclFragment() -> syntax::ASTNode;

    auto parseLambdaExpression() -> syntax::ASTNode;

    auto parseStatement() -> syntax::ASTNode;

    auto parseControlStatement() -> syntax::ASTNode;

    auto parseIfStatement() -> syntax::ASTNode;

    auto parseForStatement() -> syntax::ASTNode;

    auto parseReturnStatement() -> syntax::ASTNode;

    auto parseLambdaPrototype() -> syntax::ASTNode;

  private:
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
