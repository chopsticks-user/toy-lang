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

    auto parseClassDefinition() -> syntax::VNode;

    auto parseFunctionDefinition() -> syntax::VNode;

    auto parseFunctionPrototype() -> syntax::VNode;

    auto parseIdentifierDeclStatement() -> syntax::VNode;

    auto parseIdentifierDeclFragment() -> syntax::VNode;

    auto parseModuleStatement() -> syntax::VNode;

    auto parseImportStatement() -> syntax::VNode;

    auto parseSpecifier() -> std::string;

    auto parseVisibilitySpecifier() -> std::string;

    auto parseExpression() -> syntax::VNode;

    auto parseSequenceExpression() -> syntax::VNode;

    auto parseTernaryExpression() -> syntax::VNode;

    auto parseNullCoalescingExpression() -> syntax::VNode;

    auto parseLogicalOrExpression() -> syntax::VNode;

    auto parseLogicalAndExpression() -> syntax::VNode;

    auto parseInclusiveOrExpression() -> syntax::VNode;

    auto parseExclusiveOrExpression() -> syntax::VNode;

    auto parseAndExpression() -> syntax::VNode;

    auto parseEqualityExpression() -> syntax::VNode;

    auto parseRelationalExpression() -> syntax::VNode;

    auto parseShiftExpression() -> syntax::VNode;

    auto parseAdditiveExpression() -> syntax::VNode;

    auto parseMultiplicativeExpression() -> syntax::VNode;

    auto parsePrefixUnaryExpression() -> syntax::VNode;

    auto parsePostfixExpression() -> syntax::VNode;

    auto parsePrimaryExpression() -> syntax::VNode;

    auto parseArgumentList() -> std::vector<syntax::VNode>;

    auto parseBlockStatement() -> syntax::VNode;

    auto parseTypeExpression() -> syntax::VNode;

    auto parseParameterDeclFragment() -> syntax::VNode;

    auto parseLambdaExpression() -> syntax::VNode;

    auto parseStatement() -> syntax::VNode;

    auto parseControlStatement() -> syntax::VNode;

    auto parseIfStatement() -> syntax::VNode;

    auto parseForStatement() -> syntax::VNode;

    auto parseReturnStatement() -> syntax::VNode;

    auto parseLambdaPrototype() -> syntax::VNode;

  private:
    TokenIterator m_tokenIt;
    TokenIterator m_tokenItEnd;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
