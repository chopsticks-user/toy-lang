#ifndef  TOYLANG_FRONTEND_PARSER_HPP
#define  TOYLANG_FRONTEND_PARSER_HPP

#include "Syntax/Syntax.hpp"
#include "Lexer.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class Parser {
    using TokenIterator = Tokens::const_iterator;
    using NodeOrEmpty = std::optional<syntax::VNode>;

  public:
    auto operator()(Tokens tokens) -> syntax::VNode;

  private:
    auto current() const -> Token;

    auto match(std::same_as<EToken> auto... expected) -> bool;

    auto peekNext() -> Token;

    auto peekPrev() -> Token;

  private:
    auto parseTranslationUnit() -> syntax::TranslationUnit;

    auto parseClassDefinition() -> NodeOrEmpty;

    auto parseFunctionDefinition() -> NodeOrEmpty;

    auto parseFunctionPrototype() -> NodeOrEmpty;

    auto parseIdentifierDeclStatement() -> NodeOrEmpty;

    auto parseIdentifierDeclFragment() -> NodeOrEmpty;

    auto parseModuleStatement() -> NodeOrEmpty;

    auto parseImportStatement() -> NodeOrEmpty;

    auto parseSpecifier() -> std::string;

    auto parseVisibilitySpecifier() -> std::string;

    auto parseExpression() -> NodeOrEmpty;

    auto parseSequenceExpression() -> NodeOrEmpty;

    auto parseTernaryExpression() -> NodeOrEmpty;

    auto parseNullCoalescingExpression() -> NodeOrEmpty;

    auto parseLogicalOrExpression() -> NodeOrEmpty;

    auto parseLogicalAndExpression() -> NodeOrEmpty;

    auto parseInclusiveOrExpression() -> NodeOrEmpty;

    auto parseExclusiveOrExpression() -> NodeOrEmpty;

    auto parseAndExpression() -> NodeOrEmpty;

    auto parseEqualityExpression() -> NodeOrEmpty;

    auto parseRelationalExpression() -> NodeOrEmpty;

    auto parseShiftExpression() -> NodeOrEmpty;

    auto parseAdditiveExpression() -> NodeOrEmpty;

    auto parseMultiplicativeExpression() -> NodeOrEmpty;

    auto parsePrefixUnaryExpression() -> NodeOrEmpty;

    auto parsePostfixExpression() -> NodeOrEmpty;

    auto parsePrimaryExpression() -> NodeOrEmpty;

    auto parseArgumentList() -> std::vector<syntax::VNode>;

    auto parseBlockStatement() -> NodeOrEmpty;

    auto parseTypeExpression() -> NodeOrEmpty;

    auto parseParameterDeclFragment() -> NodeOrEmpty;

    auto parseLambdaExpression() -> NodeOrEmpty;

  private:
    TokenIterator m_tokenIt;
  };
}

#endif // TOYLANG_FRONTEND_PARSER_HPP
