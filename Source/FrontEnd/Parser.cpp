#include "Parser.hpp"

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

  auto Parser::operator()(Tokens tokens) -> syntax::VNode {
    m_tokenIt = tokens.begin();
    m_tokenItEnd = tokens.end();
    return parseTranslationUnit();
  }

  auto Parser::parseTranslationUnit() -> syntax::TranslationUnit {
    std::vector<syntax::VNode> definitions;

    while (m_tokenIt != m_tokenItEnd) {
      if (auto moduleStmt = parseModuleStatement(); !isEmpty(moduleStmt)) {
        definitions.push_back(moduleStmt);
        continue;
      }

      if (auto importStmt = parseImportStatement(); !isEmpty(importStmt)) {
        definitions.push_back(importStmt);
        continue;
      }

      if (auto idDecl = parseIdentifierDeclStatement(); !isEmpty(idDecl)) {
        definitions.push_back(idDecl);
        continue;
      }

      if (auto classDef = parseClassDefinition(); !isEmpty(classDef)) {
        definitions.push_back(classDef);
        continue;
      }

      if (auto functionDef = parseFunctionDefinition(); !isEmpty(functionDef)) {
        definitions.push_back(functionDef);
      }
    }

    return syntax::TranslationUnit(definitions);
  }

  auto Parser::parseFunctionDefinition() -> syntax::VNode {
    auto specifier = parseSpecifier();
    syntax::VNode prototype, identifier, body;
    bool pure = false;

    if (match(EToken::Fn) && match(EToken::Identifier)) {
      identifier = syntax::Identifier{peekPrev().string()};
    } else if (match(EToken::Identifier)) {
      identifier = syntax::Identifier{peekPrev().string()};

      if (!match(EToken::Colon)) {
        // todo: throw
        return {};
      }
    } else {
      return {};
    }

    prototype = parseFunctionPrototype();

    // qualifier
    if (match(EToken::Pure)) {
      pure = true;
    }

    body = parseBlockStatement();
    if (isEmpty(body)) {
      throw std::runtime_error("missing body required in parseFunctionDefinition");
    }

    return syntax::Function{identifier, prototype, body, specifier, pure};
  }

  auto Parser::parseFunctionPrototype() -> syntax::VNode {
    namespace rv = std::ranges::views;

    std::vector<syntax::VNode> paramFragments;
    syntax::VNode typeExpr;

    if (!match(EToken::LeftParen)) {
      paramFragments.push_back(parseParameterDeclFragment());

      if (match(EToken::RightParen)) {
        throw std::runtime_error("Missing ) required in parseFunctionPrototype");
      }
    }

    if (auto first = parseParameterDeclFragment(); !isEmpty(first)) {
      paramFragments.push_back(first);
      while (match(EToken::Comma)) {
        paramFragments.push_back(parseParameterDeclFragment());
      }
    }

    auto paramView = paramFragments | rv::filter([](const syntax::VNode &node) {
      return !isEmpty(node);
    });

    [[maybe_unused]] volatile bool mrp = match(EToken::RightParen);

    if (!match(EToken::MinusGreater)) {
      throw std::runtime_error("Missing -> required in parseFunctionPrototype");
    }

    typeExpr = parseTypeExpression();
    return syntax::FunctionPrototype(
      typeExpr, {paramView.begin(), paramView.end()}
    );
  }

  auto Parser::parseClassDefinition() -> syntax::VNode {
    namespace rv = std::ranges::views;

    auto visibility = parseVisibilitySpecifier();

    if (!match(EToken::Class, EToken::Interface)) {
      return {};
    }

    std::vector<syntax::VNode> parents;
    if (match(EToken::Colon)) {
      parents.push_back(parseTypeExpression());

      while (match(EToken::Comma)) {
        parents.push_back(parseTypeExpression());
      }
    }

    auto body = parseBlockStatement();

    auto parentView = parents | rv::filter([](const syntax::VNode &node) {
      return !isEmpty(node);
    });
    return syntax::Clazz(visibility, {parentView.begin(), parentView.end()}, body);
  }

  auto Parser::parseIdentifierDeclStatement() -> syntax::VNode {
    namespace rv = std::ranges::views;

    if (!match(EToken::Var, EToken::Const)) {
      return {};
    }

    std::string mutibilitySpecifier = peekPrev().string();
    std::vector decls{parseIdentifierDeclFragment()};

    while (match(EToken::Comma)) {
      decls.push_back(parseIdentifierDeclFragment());
    }

    if (!match(EToken::Semicolon)) {
      throw std::runtime_error("missing ; required in parseIdentifierDeclStatement");
    }

    auto declView = decls
                    | rv::filter([](const syntax::VNode &node) {
                      return !isEmpty(node);
                    })
                    | rv::transform([](const syntax::VNode &node) {
                      return node;
                    });
    return syntax::IdentifierDeclStatement(
      {declView.begin(), declView.end()},
      mutibilitySpecifier
    );
  }

  auto Parser::parseIdentifierDeclFragment() -> syntax::VNode {
    if (match(EToken::Identifier)) {
      auto identifier = syntax::Identifier(peekPrev().string());
      if (match(EToken::Colon)) {
        auto typeExpr = parseTypeExpression();
        syntax::VNode rhsExpr;

        if (match(EToken::Equal)) {
          rhsExpr = parseExpression();
        }

        return syntax::IdentifierDeclFragment(identifier, typeExpr, rhsExpr);
      }
    }

    return {};
  }

  auto Parser::parseModuleStatement() -> syntax::VNode {
    namespace rv = std::ranges::views;

    if (match(EToken::Module)) {
      auto fragments = std::vector<syntax::VNode>{};

      if (match(EToken::Identifier)) {
        fragments.push_back(syntax::Identifier(peekPrev().string()));
      }

      while (match(EToken::Colon2)) {
        if (match(EToken::Identifier)) {
          fragments.push_back(syntax::Identifier(peekPrev().string()));
        }
      }

      if (!match(EToken::Semicolon)) {
        throw std::runtime_error("Missing ; required in parseModuleStatement");
      }

      auto fragmentView = fragments | rv::filter(
                            [](const syntax::VNode &node) {
                              return !isEmpty(node);
                            });
      return syntax::ModuleExpr({fragmentView.begin(), fragmentView.end()});
    }

    return {};
  }

  auto Parser::parseImportStatement() -> syntax::VNode {
    namespace rv = std::ranges::views;

    if (!match(EToken::Import)) {
      return {};
    }

    auto fragments = std::vector<syntax::VNode>{};

    if (match(EToken::Identifier)) {
      fragments.push_back(syntax::Identifier(peekPrev().string()));
    }

    while (match(EToken::Colon2) && match(EToken::Identifier)) {
      fragments.push_back(syntax::Identifier(peekPrev().string()));
    }

    if (!match(EToken::Semicolon)) {
      throw std::runtime_error("Missing ; required in parseImportStatement");
    }

    auto fragmentView = fragments | rv::filter(
                          [](const syntax::VNode &node) {
                            return !isEmpty(node);
                          });
    return syntax::ImportExpr({fragmentView.begin(), fragmentView.end()});
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

  auto Parser::parseExpression() -> syntax::VNode {
    return parseSequenceExpression();
  }

  auto Parser::parseSequenceExpression() -> syntax::VNode {
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

  auto Parser::parseTernaryExpression() -> syntax::VNode {
    auto cond = parseNullCoalescingExpression();

    if (match(EToken::QMark)) {
      syntax::VNode ifTrue = parseNullCoalescingExpression();

      if (match(EToken::Colon)) {
        return syntax::TernaryExpr(
          cond, ifTrue, parseNullCoalescingExpression(), "?", ":"
        );
      }

      throw std::runtime_error("parseTernaryExpression");
    }

    return cond;
  }

  auto Parser::parseNullCoalescingExpression() -> syntax::VNode {
    auto lhs = parseLogicalOrExpression();

    while (match(EToken::QMark2)) {
      lhs = syntax::BinaryExpr(lhs, parseLogicalOrExpression(), "??");
    }

    return lhs;
  }

  auto Parser::parseLogicalOrExpression() -> syntax::VNode {
    auto lhs = parseLogicalAndExpression();

    while (match(EToken::Bar2)) {
      lhs = syntax::BinaryExpr(lhs, parseLogicalAndExpression(), "||");
    }

    return lhs;
  }

  auto Parser::parseLogicalAndExpression() -> syntax::VNode {
    auto lhs = parseInclusiveOrExpression();

    while (match(EToken::Ampersand2)) {
      lhs = syntax::BinaryExpr(lhs, parseInclusiveOrExpression(), "&&");
    }

    return lhs;
  }

  auto Parser::parseInclusiveOrExpression() -> syntax::VNode {
    auto lhs = parseExclusiveOrExpression();

    while (match(EToken::Bar)) {
      lhs = syntax::BinaryExpr(lhs, parseExclusiveOrExpression(), "|");
    }

    return lhs;
  }

  auto Parser::parseExclusiveOrExpression() -> syntax::VNode {
    auto lhs = parseAndExpression();

    while (match(EToken::Hat)) {
      lhs = syntax::BinaryExpr(lhs, parseAndExpression(), "^");
    }

    return lhs;
  }

  auto Parser::parseAndExpression() -> syntax::VNode {
    auto lhs = parseEqualityExpression();

    while (match(EToken::Ampersand)) {
      lhs = syntax::BinaryExpr(lhs, parseEqualityExpression(), "&");
    }

    return lhs;
  }

  auto Parser::parseEqualityExpression() -> syntax::VNode {
    auto lhs = parseRelationalExpression();

    while (match(EToken::Equal2, EToken::BarEqual)) {
      lhs = syntax::BinaryExpr(lhs, parseRelationalExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseRelationalExpression() -> syntax::VNode {
    auto lhs = parseShiftExpression();

    while (match(EToken::Less, EToken::LessEqual, EToken::Greater, EToken::GreaterEqual)) {
      lhs = syntax::BinaryExpr(lhs, parseShiftExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseShiftExpression() -> syntax::VNode {
    auto lhs = parseAdditiveExpression();

    while (match(EToken::Less2, EToken::Greater2)) {
      lhs = syntax::BinaryExpr(lhs, parseAdditiveExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseAdditiveExpression() -> syntax::VNode {
    auto lhs = parseMultiplicativeExpression();

    while (match(EToken::Plus, EToken::Minus)) {
      lhs = syntax::BinaryExpr(lhs, parseMultiplicativeExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parseMultiplicativeExpression() -> syntax::VNode {
    auto lhs = parsePrefixUnaryExpression();

    while (match(EToken::Star, EToken::FwdSlash, EToken::Percent)) {
      lhs = syntax::BinaryExpr(lhs, parsePrefixUnaryExpression(), peekPrev().string());
    }

    return lhs;
  }

  auto Parser::parsePrefixUnaryExpression() -> syntax::VNode {
    auto operand = parsePostfixExpression();

    if (match(EToken::Exclaim, EToken::Tilde, EToken::Plus, EToken::Minus)) {
      operand = syntax::UnaryExpr(operand, peekPrev().string());
    }

    return operand;
  }

  auto Parser::parsePostfixExpression() -> syntax::VNode {
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
      auto args = parseArgumentList();

      if (match(EToken::RightParen)) {
        return syntax::FunctionCallExpr(operand, args);
      }

      throw std::runtime_error(
        "missing ) required for function-call expression in parsePostfixExpression"
      );
    }

    // pipe expression
    if (match(EToken::BarGreater)) {
      auto op = peekPrev().string();
      auto expr = parseExpression();

      return syntax::BinaryExpr(operand, expr, op);
    }

    // post-fix unary operator
    if (match(EToken::Plus2, EToken::Minus2)) {
      return syntax::PostfixUnaryExpr(operand, peekPrev().string());
    }

    return operand;
  }

  auto Parser::parsePrimaryExpression() -> syntax::VNode {
    if (auto lambda = parseLambdaExpression(); !isEmpty(lambda)) {
      return lambda;
    }

    if (match(EToken::LeftParen)) {
      if (match(EToken::RightParen)) {
        return parseExpression();
      }

      throw std::runtime_error("missing ) required in parsePrimaryExpression");
    }

    if (auto typeExpr = parseTypeExpression(); !isEmpty(typeExpr)) {
      return typeExpr;
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

    if (match(EToken::Identifier, EToken::Self)) {
      return syntax::Identifier(peekPrev().string());
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
                        | rv::filter([](const syntax::VNode &node) {
                          return !isEmpty(node);
                        });
    return {argumentView.begin(), argumentView.end()};
  }

  auto Parser::parseBlockStatement() -> syntax::VNode {
    if (!match(EToken::LeftBrace)) {
      return {};
    }

    std::vector<syntax::VNode> statements;
    for (auto stmt = parseStatement();
         !isEmpty(stmt);
         stmt = parseStatement()
    ) {
      statements.push_back(stmt);
      if (match(EToken::RightBrace)) {
        break;
      }
    }

    if (peekPrev().type() != EToken::RightBrace) {
      throw std::runtime_error("Missing } required for parseBlockStatement");
    }

    return syntax::BlockStatement(statements);
  }

  auto Parser::parseTypeExpression() -> syntax::VNode {
    if (match(EToken::FundamentalType, EToken::UserDefinedType)) {
      return syntax::TypeExpr(peekPrev().string());
    }

    return {};
  }

  auto Parser::parseParameterDeclFragment() -> syntax::VNode {
    std::string mutibility = "const";
    if (match(EToken::Var, EToken::Const)) {
      mutibility = peekPrev().string();
    }

    if (auto idDecl = parseIdentifierDeclFragment(); !isEmpty(idDecl)) {
      return syntax::ParameterDeclFragment{idDecl, mutibility};
    }

    return {};
  }

  auto Parser::parseLambdaExpression() -> syntax::VNode {
    auto prototype = parseLambdaPrototype();

    if (isEmpty(prototype)) {
      return {};
    }

    bool pure = false;
    auto identifier = syntax::Identifier("lambda"); // todo:

    if (match(EToken::Pure)) {
      pure = true;
    }

    return syntax::Function(
      identifier, prototype, parseBlockStatement(), "", pure
    );
  }

  auto Parser::parseStatement() -> syntax::VNode {
    if (auto control = parseControlStatement(); !isEmpty(control)) {
      return control;
    }

    if (auto idDecl = parseIdentifierDeclStatement(); !isEmpty(idDecl)) {
      return idDecl;
    }

    if (auto block = parseBlockStatement(); !isEmpty(block)) {
      return block;
    }

    if (auto returnStmt = parseReturnStatement(); !isEmpty(returnStmt)) {
      return returnStmt;
    }

    auto expr = parseExpression();

    // empty statement
    if (isEmpty(expr)) {
      if (!match(EToken::Semicolon)) {
        throw std::runtime_error("Missing ; required in parseStatement");
      }

      return {};
    }

    // assignment statement
    if (match(EToken::StarEqual, EToken::FwdSlashEqual, EToken::PercentEqual, EToken::PlusEqual,
              EToken::MinusEqual, EToken::Greater2Equal, EToken::Less2Equal, EToken::AmpersandEqual,
              EToken::HatEqual, EToken::BarEqual, EToken::Equal)) {
      auto lhs = parseExpression();
      auto assignmentStmt = syntax::AssignmentStatement(expr, lhs, peekPrev().string());

      if (!match(EToken::Semicolon)) {
        throw std::runtime_error("Missing ; required in parseStatement");
      }

      return assignmentStmt;
    }

    if (!match(EToken::Semicolon)) {
      throw std::runtime_error("Missing ; required in parseStatement");
    }

    // expression statement
    return expr;
  }

  auto Parser::parseControlStatement() -> syntax::VNode {
    if (auto ifStmt = parseIfStatement(); !isEmpty(ifStmt)) {
      return ifStmt;
    }

    if (auto forStmt = parseForStatement(); !isEmpty(forStmt)) {
      return forStmt;
    }

    return {};
  }

  auto Parser::parseIfStatement() -> syntax::VNode {
    if (!match(EToken::If)) {
      return {};
    }

    // optional parenthesis, marked volatile to avoid being optimized away (maybe)
    [[maybe_unused]] volatile bool mlp = match(EToken::LeftParen);

    auto idDecl = parseIdentifierDeclStatement();
    auto cond = parseExpression();

    // optional parenthesis, marked volatile to avoid being optimized away (maybe)
    [[maybe_unused]] volatile bool mrp = match(EToken::RightParen);

    auto body = parseBlockStatement();
    syntax::VNode elseBody;

    if (match(EToken::Else)) {
      elseBody = parseBlockStatement();
      if (!isEmpty(elseBody)) {
        elseBody = parseIfStatement();
      }
    }

    return syntax::IfStatement(idDecl, cond, body, elseBody);
  }

  auto Parser::parseForStatement() -> syntax::VNode {
    if (!match(EToken::For)) {
      return {};
    }

    // optional parenthesis, marked volatile to avoid being optimized away (maybe)
    [[maybe_unused]] volatile bool mlp = match(EToken::LeftParen);

    auto idDecl = parseIdentifierDeclStatement();

    // regular for loop
    if (!isEmpty(idDecl)) {
      // if (!match(EToken::Semicolon)) {
      //   throw std::runtime_error("missing ; required in parseForStatement");
      // }

      auto cond = parseExpression();

      if (!match(EToken::Semicolon)) {
        throw std::runtime_error("missing ; required in parseForStatement");
      }

      auto postExpr = parseExpression();

      // optional parenthesis, marked volatile to avoid being optimized away (maybe)
      [[maybe_unused]] volatile bool mrp = match(EToken::RightParen);

      return syntax::ForStatement(idDecl, cond, postExpr, parseBlockStatement());
    }

    // range-for loop
    if (!match(EToken::Colon)) {
      throw std::runtime_error("missing : required in parseForStatement");
    }

    auto collection = parseExpression();

    // optional parenthesis, marked volatile to avoid being optimized away (maybe)
    [[maybe_unused]] volatile bool mrp = match(EToken::RightParen);

    return syntax::ForStatement(idDecl, collection, parseBlockStatement());
  }

  auto Parser::parseReturnStatement() -> syntax::VNode {
    if (!match(EToken::Return)) {
      return {};
    }

    // return void
    if (match(EToken::Semicolon)) {
      return syntax::ReturnStatement({});
    }

    auto expr = parseExpression();

    if (!match(EToken::Semicolon)) {
      throw std::runtime_error("Missing ; required in parseReturnStatement");
    }

    return syntax::ReturnStatement(expr);
  }

  auto Parser::parseLambdaPrototype() -> syntax::VNode {
    namespace rv = std::ranges::views;

    // for recovering in case this is not a lambda expression
    const auto it = m_tokenIt;

    std::vector<syntax::VNode> paramFragments;
    syntax::VNode typeExpr;

    if (!match(EToken::LeftParen)) {
      paramFragments.push_back(parseParameterDeclFragment());

      if (match(EToken::RightParen)) {
        m_tokenIt = it;
        return {};
      }
    }

    paramFragments.push_back(parseParameterDeclFragment());
    while (match(EToken::Comma)) {
      paramFragments.push_back(parseParameterDeclFragment());
    }

    auto paramView = paramFragments
                     | rv::filter([](const syntax::VNode &node) {
                       return !isEmpty(node);
                     })
                     | rv::transform([](const syntax::VNode &node) {
                       return node;
                     });

    [[maybe_unused]] volatile bool mrp = match(EToken::RightParen);

    if (!match(EToken::MinusGreater)) {
      m_tokenIt = it;
      return {};
    }

    typeExpr = parseTypeExpression();
    return syntax::FunctionPrototype(
      typeExpr, {paramView.begin(), paramView.end()}
    );
  }
}
