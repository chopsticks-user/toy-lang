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
    NodeOrEmpty prototype, identifier, body;
    bool pure = false;

    if (match(EToken::Fn) && match(EToken::Identifier)) {
      identifier = syntax::Identifier{peekPrev().string()};
    } else if (match(EToken::Identifier)) {
      identifier = syntax::Identifier{peekPrev().string()};

      if (!match(EToken::Colon)) {
        // todo: throw
        return {};
      }
    }

    prototype = parseFunctionPrototype();

    // qualifier
    if (match(EToken::Pure)) {
      pure = true;
    }

    body = parseBlockStatement();
    if (!body.has_value()) {
      body = parseExpression();

      if (!match(EToken::Semicolon)) {
        return {};
      }
    }

    return syntax::Function{identifier, prototype, body, specifier, pure};
  }

  auto Parser::parseFunctionPrototype() -> NodeOrEmpty {
    namespace rv = std::ranges::views;

    if (match(EToken::LeftParen)) {
      std::vector paramFragments{parseParameterDeclFragment()};
      while (match(EToken::Comma)) {
        paramFragments.push_back(parseParameterDeclFragment());
      }

      auto paramView = paramFragments
                       | rv::filter([](const NodeOrEmpty &node) {
                         return node.has_value();
                       })
                       | rv::transform([](const NodeOrEmpty &node) {
                         return node.value();
                       });

      if (match(EToken::RightParen) && match(EToken::MinusGreater)) {
        auto typeExpr = parseTypeExpression();
        return syntax::FunctionPrototype(
          typeExpr, {paramView.begin(), paramView.end()}
        );
      }
    }

    auto singleParam = parseParameterDeclFragment().value();
    if (match(EToken::MinusGreater)) {
      auto typeExpr = parseTypeExpression();
      return syntax::FunctionPrototype(typeExpr, {singleParam});
    }

    return {};
  }

  auto Parser::parseClassDefinition() -> NodeOrEmpty {
  }

  auto Parser::parseIdentifierDeclStatement() -> NodeOrEmpty {
    namespace rv = std::ranges::views;

    std::string mutibilitySpecifier = "const";
    if (match(EToken::Var, EToken::Const)) {
      mutibilitySpecifier = peekPrev().string();
    }

    std::vector decls{parseIdentifierDeclFragment()};
    while (match(EToken::Comma)) {
      decls.push_back(parseIdentifierDeclFragment());
    }

    auto declView = decls
                    | rv::filter([](const NodeOrEmpty &node) {
                      return node.has_value();
                    })
                    | rv::transform([](const NodeOrEmpty &node) {
                      return node.value();
                    });
    return syntax::IdentifierDeclStatement(
      {declView.begin(), declView.end()},
      mutibilitySpecifier
    );
  }

  auto Parser::parseIdentifierDeclFragment() -> NodeOrEmpty {
    if (match(EToken::Identifier)) {
      auto identifier = syntax::Identifier(peekPrev().string());
      if (match(EToken::Colon)) {
        auto typeExpr = parseTypeExpression();
        NodeOrEmpty rhsExpr;

        if (match(EToken::Equal)) {
          rhsExpr = parseExpression();
        }

        return syntax::IdentifierDeclFragment(identifier, typeExpr, rhsExpr);
      }
    }

    return {};
  }

  auto Parser::parseModuleStatement() -> NodeOrEmpty {
  }

  auto Parser::parseImportStatement() -> NodeOrEmpty {
  }

  auto Parser::parseSpecifier() -> std::string {
    return parseVisibilitySpecifier();
  }

  auto Parser::parseVisibilitySpecifier() -> std::string {
    if (match(EToken::Export, EToken::Internal, EToken::Local)) {
      return peekPrev().string();
    }
    return "";
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
    if (match(EToken::Identifier, EToken::Self)) {
      return syntax::Identifier(peekPrev().string());
    }

    if (match(EToken::FundamentalType, EToken::UserDefinedType)) {
      return syntax::TypeExpr(peekPrev().string());
    }

    if (match(EToken::IntegerLiteral)) {
      return syntax::IntegerLiteral(peekPrev().string());
    }

    if (match(EToken::FloatLiteral)) {
      return syntax::FloatLiteral(peekPrev().string());
    }

    if (match(EToken::StringLiteral)) {
      return syntax::StringLiteral(peekPrev().string());
    }

    if (match(EToken::LeftParen)) {
      if (match(EToken::RightParen)) {
        return parseExpression();
      }

      throw std::runtime_error("parenthesis expression");
    }

    throw std::runtime_error("unknown primary expression");
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

  auto Parser::parseBlockStatement() -> NodeOrEmpty {
    return {};
  }

  auto Parser::parseTypeExpression() -> NodeOrEmpty {
    if (match(EToken::FundamentalType, EToken::UserDefinedType)) {
      return syntax::TypeExpr(peekPrev().string());
    }

    return {};
  }

  auto Parser::parseParameterDeclFragment() -> NodeOrEmpty {
    std::string mutibility = "const";
    if (match(EToken::Var, EToken::Const)) {
      mutibility = peekPrev().string();
    }

    return syntax::ParameterDeclFragment{parseIdentifierDeclFragment(), mutibility};
  }

  auto Parser::parseLambdaExpression() -> NodeOrEmpty {
    auto prototype = parseFunctionPrototype();
    bool pure = false;
    auto identifier = syntax::Identifier("lambda"); // todo:

    if (match(EToken::Pure)) {
      pure = true;
    }

    return syntax::Function(
      identifier, prototype, parseBlockStatement(), "", pure
    );
  }
}
