#include "Parser.hpp"

namespace tl::fe {
  auto Parser::current() const -> Token {
    return *m_tokenIt;
  }

  template<typename... T>
  auto Parser::match(std::same_as<EToken> auto... expected) -> bool {
    ++m_tokenIt;

    if (((m_tokenIt->type() == expected) || ...)) {
      return true;
    }

    --m_tokenIt;
    return false;
  }

  auto Parser::peekNext() -> Token {
    ++m_tokenIt;
    return *(m_tokenIt--);
  }

  auto Parser::peekPrev() -> Token {
    --m_tokenIt;
    return *(m_tokenIt++);
  }

  auto Parser::operator()(Tokens tokens) -> syntax::VNode {
    m_tokenIt = tokens.begin();
    return parseTranslationUnit().value();
  }

  auto Parser::parseTranslationUnit() -> NodeOrEmpty {
  }

  auto Parser::parseFunctionDefinition() -> NodeOrEmpty {
    auto specifier = parseSpecifier();
  }

  auto Parser::parseClassDefinition() -> NodeOrEmpty {
  }

  auto Parser::parseIdentifierDeclStatement() -> NodeOrEmpty {
  }

  auto Parser::parseModuleStatement() -> NodeOrEmpty {
  }

  auto Parser::parseImportStatement() -> NodeOrEmpty {
  }

  auto Parser::parseSpecifier() -> std::string {
    return parseVisibilitySpecifier();
  }

  auto Parser::parseVisibilitySpecifier() -> std::string {
    return current().string();
  }

  auto Parser::parseExpression() -> NodeOrEmpty {
    return parseSequenceExpression();
  }

  auto Parser::parseSequenceExpression() -> NodeOrEmpty {
    auto from = parseTernaryExpression();

    if (match(EToken::Dot2)) {
      auto to = parseTernaryExpression();

      if (match(EToken::By)) {
        return syntax::TernaryExpr(
          from, to, parseTernaryExpression(), "..", " by"
        );
      }

      throw std::runtime_error("parseSequenceExpression");
    }

    return from;
  }

  auto Parser::parseTernaryExpression() -> NodeOrEmpty {
    auto cond = parseNullCoalescingExpression();

    if (match(EToken::QMark)) {
      NodeOrEmpty ifTrue = parseNullCoalescingExpression();

      if (match(EToken::Colon)) {
        return syntax::TernaryExpr(
          cond, ifTrue, parseNullCoalescingExpression(), "?", ":"
        );
      }

      throw std::runtime_error("parseTernaryExpression");
    }

    return cond;
  }

  auto Parser::parseNullCoalescingExpression() -> NodeOrEmpty {
    auto lhs = parseLogicalOrExpression();

    while (match(EToken::QMark2)) {
      lhs = syntax::BinaryExpr(lhs, parseLogicalOrExpression(), "??");
    }

    return lhs;
  }

  auto Parser::parseLogicalOrExpression() -> NodeOrEmpty {
    auto lhs = parseLogicalAndExpression();

    while (match(EToken::Bar2)) {
      lhs = syntax::BinaryExpr(lhs, parseLogicalAndExpression(), "||");
    }

    return lhs;
  }

  auto Parser::parseLogicalAndExpression() -> NodeOrEmpty {
    auto lhs = parseInclusiveOrExpression();

    while (match(EToken::Ampersand2)) {
      lhs = syntax::BinaryExpr(lhs, parseInclusiveOrExpression(), "&&");
    }

    return lhs;
  }

  auto Parser::parseInclusiveOrExpression() -> NodeOrEmpty {
    auto lhs = parseExclusiveOrExpression();

    while (match(EToken::Bar)) {
      lhs = syntax::BinaryExpr(lhs, parseExclusiveOrExpression(), "|");
    }

    return lhs;
  }

  auto Parser::parseExclusiveOrExpression() -> NodeOrEmpty {
    auto lhs = parseAndExpression();

    while (match(EToken::Hat)) {
      lhs = syntax::BinaryExpr(lhs, parseAndExpression(), "^");
    }

    return lhs;
  }

  auto Parser::parseAndExpression() -> NodeOrEmpty {
    auto lhs = parseEqualityExpression();

    while (match(EToken::Ampersand)) {
      lhs = syntax::BinaryExpr(lhs, parseEqualityExpression(), "&");
    }

    return lhs;
  }

  auto Parser::parseEqualityExpression() -> NodeOrEmpty {
    auto lhs = parseRelationalExpression();

    while (match(EToken::Equal2, EToken::BarEqual)) {
      lhs = syntax::BinaryExpr(lhs, parseRelationalExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseRelationalExpression() -> NodeOrEmpty {
    auto lhs = parseShiftExpression();

    while (match(EToken::Less, EToken::LessEqual, EToken::Greater, EToken::GreaterEqual)) {
      lhs = syntax::BinaryExpr(lhs, parseShiftExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseShiftExpression() -> NodeOrEmpty {
    auto lhs = parseAdditiveExpression();

    while (match(EToken::Less2, EToken::Greater2)) {
      lhs = syntax::BinaryExpr(lhs, parseAdditiveExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseAdditiveExpression() -> NodeOrEmpty {
    auto lhs = parseMultiplicativeExpression();

    while (match(EToken::Plus, EToken::Minus)) {
      lhs = syntax::BinaryExpr(lhs, parseMultiplicativeExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseMultiplicativeExpression() -> NodeOrEmpty {
    auto lhs = parsePrefixUnaryExpression();

    while (match(EToken::Star, EToken::FwdSlash, EToken::Percent)) {
      lhs = syntax::BinaryExpr(lhs, parsePrefixUnaryExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parsePrefixUnaryExpression() -> NodeOrEmpty {
    auto operand = parsePostfixExpression();

    if (match(EToken::Exclaim, EToken::Tilde, EToken::Plus, EToken::Minus)) {
      operand = syntax::UnaryExpr(operand, peekPrev().string());
    }

    return operand;
  }

  auto Parser::parsePostfixExpression() -> NodeOrEmpty {
    auto operand = parsePrimaryExpression();

    // array-subscripting expression
    if (match(EToken::LeftBracket)) {
      if (match(EToken::RightBracket)) {
        return syntax::SubScriptingExpr(operand, parseExpression());
      }

      throw std::runtime_error("array-subscripting expression");
    }

    // function-call expression
    if (match(EToken::LeftParen)) {
      if (match(EToken::RightParen)) {
        return syntax::FunctionCallExpr(operand, parseArgumentList());
      }

      throw std::runtime_error("function-call expression");
    }

    // pipe expression
    if (match(EToken::BarGreater)) {
      auto op = peekPrev().string();
      auto expr = parseExpression();

      return syntax::BinaryExpr(operand, expr, op);
    }
  }

  auto Parser::parsePrimaryExpression() -> NodeOrEmpty {
  }

  auto Parser::parseArgumentList() -> std::vector<syntax::VNode> {
    namespace rv = std::ranges::views;

    std::vector arguments{parseExpression()};

    while (match(EToken::Comma)) {
      arguments.push_back(parseExpression());
    }

    auto argumentView = arguments
                        | rv::filter([](const NodeOrEmpty &node) {
                          return node.has_value();
                        })
                        | rv::transform([](const NodeOrEmpty &node) {
                          return node.value();
                        });
    return {argumentView.begin(), argumentView.end()};
  }
}
