#include "Parser.hpp"

#include <utility>

using tl::syntax::ASTNode;
using tl::syntax::isEmptyAst;

namespace tl::fe {
  auto Parser::current() const -> CRef<Token> {
    if (!m_currentToken.has_value()) {
      throw InternalException(m_filepath, "Parser: m_currentToken == nullptr");
    }
    return *m_currentToken;
  }

  auto Parser::revert() -> void {
    --m_tokenIt;
    m_currentToken = *m_tokenIt;
  }

  auto Parser::advance() -> void {
    ++m_tokenIt;
    m_currentToken = *m_tokenIt;
  }

  auto Parser::match(std::same_as<EToken> auto... expected) -> bool {
    if (m_tokenIt == m_tokenItEnd) {
      return false;
    }

    if (!((m_tokenIt->type() == expected) || ...)) {
      return false;
    }

    m_currentToken = *m_tokenIt;
    ++m_tokenIt;
    return true;
  }

  auto Parser::peekNext() -> Token {
    return *m_tokenIt;
  }

  auto Parser::peekPrev() -> Token {
    --m_tokenIt;
    return *(m_tokenIt++);
  }

  auto Parser::collectException(ToyLangException &&e) const -> void {
    m_eCollector->add(std::move(e));
  }

  auto Parser::collectException(CRef<String> mesg) const -> void {
    collectException(
      ParserExpception(m_filepath, current(), mesg)
    );
  }

  auto Parser::setStorage(const syntax::Storage storage) -> void {
    m_currentStorage = storage;
  }

  auto Parser::storage() const noexcept -> syntax::Storage {
    return m_currentStorage;
  }

  auto Parser::setContext(const Context context) noexcept -> void {
    if (m_parseContext.empty()) {
      return enterContext(context);
    }

    m_parseContext.top() = context;
  }

  auto Parser::enterContext(const Context context) noexcept -> void {
    m_parseContext.push(context);
  }

  auto Parser::exitContext() noexcept -> void {
    m_parseContext.pop();
  }

  auto Parser::context() const noexcept -> Context {
    return m_parseContext.empty() ? Context::None : m_parseContext.top();
  }

  auto Parser::markRevertPoint() noexcept -> void {
    m_revertPoint = m_tokenIt;
  }

  auto Parser::toRevertPoint() noexcept -> void {
    if (!m_revertPoint.has_value()) {
      return;
    }

    m_tokenIt = m_revertPoint.value();
    m_currentToken = *m_tokenIt;
    m_revertPoint = {};
  }

  auto Parser::operator()(
    ExpceptionCollector &eCollector, String filepath, Tokens tokens
  ) -> syntax::TranslationUnit {
    m_filepath = std::move(filepath);
    m_tokenIt = tokens.begin();
    m_tokenItEnd = tokens.end();
    m_eCollector = &eCollector;

    if (m_tokenIt == m_tokenItEnd) {
      collectException(
        ParserExpception(filepath, "empty file is not allowed; module declaration must be specified")
      );
    }

    return parseTranslationUnit();
  }

  auto Parser::parseTranslationUnit() -> syntax::TranslationUnit {
    std::vector<ASTNode> definitions;

    if (const auto moduleDecl = parseModuleDecl(); !isEmptyAst(moduleDecl)) {
      definitions.push_back(moduleDecl);
    } else {
      throw ParserExpception(m_filepath, current(), "module declaration must be specified");
    }

    while (true) {
      const auto importDecl = parseImportDecl();
      if (isEmptyAst(importDecl)) {
        break;
      }
      definitions.push_back(importDecl);
    }

    while (m_tokenIt != m_tokenItEnd) {
      ASTNode definition;

      if (match(EToken::Export, EToken::Internal, EToken::Local)) {
        switch (current().type()) {
          case EToken::Export: {
            setStorage(syntax::Storage::Export);
            break;
          }
          case EToken::Local: {
            setStorage(syntax::Storage::Local);
            break;
          }
          default: {
            setStorage(syntax::Storage::Internal);
            break;
          }
        }
      }

      setContext(Context::Function);
      definition = parseFunctionDef();

      if (isEmptyAst(definition)) {
        setContext();
        definition = parseTypeDecl();
      }

      if (isEmptyAst(definition)) {
        setContext(Context::Class);
        definition = parseClassDef();
      }

      if (isEmptyAst(definition)) {
        setContext(Context::Concept);
        definition = parseConceptDef();
      }

      // todo: skip current line
      if (isEmptyAst(definition)) {
        throw ParserExpception(m_filepath, current(), "unknown global definition");
      }

      definitions.push_back(definition);
      setContext();
      setStorage();
    }

    return syntax::TranslationUnit(definitions);
  }

  auto Parser::parseModuleDecl() -> ASTNode {
    if (!match(EToken::Module)) {
      return {};
    }

    if (const auto identifier = parseIdentifier();
      !isEmptyAst(identifier)) {
      if (!match(EToken::Semicolon)) {
        collectException("missing ; at the end of module declaration");
      }

      return syntax::ModuleDecl{identifier};
    }

    collectException("module path must be specified");
    return {};
  }

  auto Parser::parseImportDecl() -> ASTNode {
    if (!match(EToken::Import)) {
      return {};
    }

    if (const auto identifier = parseIdentifier();
      !isEmptyAst(identifier)) {
      if (!match(EToken::Semicolon)) {
        collectException("missing ; at the end of import declaration");
      }

      return syntax::ImportDecl{identifier};
    }

    collectException("import path must be specified");
    return {};
  }

  auto Parser::parseTypeDecl() -> ASTNode {
    if (!match(EToken::Type)) {
      return {};
    }

    const auto typeId = parseTypeIdentifier();
    if (isEmptyAst(typeId)) {
      collectException("missing lhs type identifier for type declaration");
    }

    if (syntax::astCast<syntax::Identifier>(typeId).isImported()) {
      collectException("imported type cannot be re-defined");
    }

    if (!match(EToken::Equal)) {
      collectException("missing = for type declaration");
    }

    const auto typeExpr = parseTypeExpr();
    if (isEmptyAst(typeExpr)) {
      collectException("missing rhs type expression for type declaration");
    }

    if (!match(EToken::Semicolon)) {
      collectException("missing ; at the end of type declaration");
    }

    return syntax::TypeDecl{storage(), typeId, typeExpr};
  }

  auto Parser::parseEnumDecl() -> ASTNode {
    if (!match(EToken::Enum)) {
      return {};
    }

    return {};
  }

  auto Parser::parseFlagDecl() -> ASTNode {
    if (!match(EToken::Flag)) {
      return {};
    }

    return {};
  }

  auto Parser::parseFunctionDef() -> ASTNode {
    const auto prototype = parseFunctionPrototype();
    if (isEmptyAst(prototype)) {
      return {};
    }

    const auto body = parseBlockStmt();
    if (isEmptyAst(body)) {
      collectException("missing function body");
    }

    return syntax::FunctionDef{storage(), prototype, body};
  }

  auto Parser::parseClassDef() -> ASTNode {
    if (!match(EToken::Class)) {
      return {};
    }

    return {};
  }

  auto Parser::parseConceptDef() -> ASTNode {
    if (!match(EToken::Concept)) {
      return {};
    }

    const auto conceptId = parseTypeIdentifier();
    if (isEmptyAst(conceptId)) {
      collectException("missing concept identifier");
    }
    if (const auto id = syntax::astCast<syntax::Identifier>(conceptId);
      !id.isType() || id.isImported()) {
      collectException("invalid concept identifier");
    }

    if (!match(EToken::LeftBrace)) {
      collectException("missing '{' in concept definition");
    }

    Vec<ASTNode> requirements;
    auto requirement = parseFunctionPrototype();
    while (!isEmptyAst(requirement)) {
      requirements.push_back(requirement);

      if (!match(EToken::Semicolon)) {
        collectException("missing ; at the end of concept requirement");
      }

      requirement = parseFunctionPrototype();
    }

    if (!match(EToken::RightBrace)) {
      collectException("missing '}' in concept definition");
    }

    return syntax::ConceptDef{storage(), conceptId, requirements};
  }

  auto Parser::parseIdentifier() -> ASTNode {
    markRevertPoint();

    if (match(EToken::AnnonymousIdentifier)) {
      return syntax::Identifier{{}};
    }

    Vec<String> path;
    while (match(EToken::Identifier)) {
      const auto id = current();

      path.push_back(id.string());
      if (!match(EToken::Colon2)) {
        break;
      }
    }

    // unparsed identifier
    if (peekPrev().type() == EToken::Colon2 || path.empty()) {
      toRevertPoint();
      return {};
    }

    return syntax::Identifier{path};
  }

  auto Parser::parseTypeIdentifier() -> ASTNode {
    markRevertPoint();

    Vec<String> path;
    while (match(EToken::Identifier)) {
      const auto id = current();

      path.push_back(id.string());
      if (!match(EToken::Colon2)) {
        break;
      }
    }

    if (match(EToken::UserDefinedType, EToken::FundamentalType)) {
      path.push_back(current().string());
      return syntax::Identifier{path};
    }

    toRevertPoint();
    return {};
  }

  auto Parser::parseTypeExpr() -> ASTNode {
    Vec<ASTNode> types;

    do {
      if (auto type = parseTypeIdentifier(); !isEmptyAst(type)) {
        types.emplace_back(type);
      }
    } while (match(EToken::Bar));

    if (types.empty()) {
      return {};
    }

    return syntax::TypeExpr{types};
  }

  auto Parser::parseFunctionPrototype() -> ASTNode {
    const bool parsingLambda = context() == Context::Lambda;
    ASTNode identifier;
    if (!parsingLambda) {
      if (!match(EToken::Fn)) {
        return {};
      }

      identifier = parseIdentifier();

      // todo
      if (isEmptyAst(identifier)) {
        if ((context() == Context::Concept || context() == Context::Class) &&
            syntax::overloadableOps.contains(current().string())) {
          identifier = syntax::Identifier{{current().string()}};
          advance();
        } else {
          collectException("missing function identifier");
        }
      }

      if (!match(EToken::Colon)) {
        collectException("missing colon after function identifier");
      }
    }

    auto params = parseTupleDecl();
    if (isEmptyAst(params)) {
      params = parseIdentifierDecl();
    }
    if (isEmptyAst(params)) {
      params = parseTypeExpr();
    }
    if (isEmptyAst(params)) {
      if (parsingLambda) {
        return {};
      }
      collectException("missing function parameters");
    }

    if (!match(EToken::MinusGreater)) {
      if (parsingLambda) {
        return {};
      }
      collectException("missing -> after function parameters");
    }

    auto returns = parseTypeExpr();
    if (isEmptyAst(returns)) {
      returns = parseTupleDecl();
    }
    if (isEmptyAst(returns)) {
      returns = parseIdentifierDecl();
    }
    // if (isEmptyAst(returns)) {
    //   collectException("missing function returns");
    // }

    return syntax::FunctionPrototype{
      syntax::FnType::Function, identifier, params, returns
    };
  }

  auto Parser::parseTupleDecl() -> ASTNode {
    if (!match(EToken::LeftParen)) {
      return {};
    }

    if (match(EToken::RightParen)) {
      return syntax::TupleDecl{{}};
    }

    Vec<ASTNode> declNodes;
    do {
      auto decl = parseIdentifierDecl();
      if (!isEmptyAst(decl)) {
        declNodes.emplace_back(decl);
        continue;
      }

      decl = parseTypeExpr();
      if (!isEmptyAst(decl)) {
        declNodes.emplace_back(syntax::IdentifierDecl{{}, decl, false});
        continue;
      }

      decl = parseTupleDecl();
      if (!isEmptyAst(decl)) {
        declNodes.emplace_back(decl);
        continue;
      }

      if (isEmptyAst(decl)) {
        collectException("invalid declaration");
      }
    } while (match(EToken::Comma));

    if (!match(EToken::RightParen)) {
      collectException("missing ')' for tuple declaration");
    }

    return syntax::TupleDecl{std::move(declNodes)};
  }

  auto Parser::parseIdentifierDecl() -> ASTNode {
    markRevertPoint();

    const bool isMutable = match(EToken::Mutable);
    const auto identifier = parseIdentifier();

    if (isEmptyAst(identifier)) {
      toRevertPoint();
      return {};
    }

    if (match(EToken::Colon)) {
      const auto typeExpr = parseTypeExpr();

      if (isEmptyAst(typeExpr)) {
        collectException("missing ')' for function parameter declaration");
      }

      return syntax::IdentifierDecl{identifier, typeExpr, isMutable};
    }

    return syntax::IdentifierDecl{identifier, {}, isMutable};
  }

  auto Parser::parseStmt() -> ASTNode {
    if (const auto forStmt = parseForStmt(); !isEmptyAst(forStmt)) {
      return forStmt;
    }

    if (const auto matchStmt = parseMatchStmt(); !isEmptyAst(matchStmt)) {
      return matchStmt;
    }

    if (const auto blockStmt = parseBlockStmt(); !isEmptyAst(blockStmt)) {
      return blockStmt;
    }

    if (const auto letStmt = parseLetStmt(); !isEmptyAst(letStmt)) {
      return letStmt;
    }

    if (const auto returnStmt = parseReturnStmt(); !isEmptyAst(returnStmt)) {
      return returnStmt;
    }

    // must be last
    if (const auto stmt = parseExprPrefixStmt(); !isEmptyAst(stmt)) {
      return stmt;
    }

    // collectException("unknown statement");
    return {};
  }

  auto Parser::parseForStmt() -> ASTNode {
    if (!match(EToken::For)) {
      return {};
    }

    ASTNode condition = {};

    if (const auto iterator = parseTupleDecl(); !isEmptyAst(iterator)) {
      if (!match(EToken::In)) {
        collectException("missing 'in'");
      }

      const auto iterable = parseExpr();
      if (isEmptyAst(iterable)) {
        collectException("missing iterable expression");
      }

      condition = syntax::ForRangeFragment{iterator, iterable};
    } else {
      condition = parseExpr();
      if (isEmptyAst(condition)) {
        collectException("missing conditional expression of for statement");
      }
    }


    const auto body = parseBlockStmt();
    if (isEmptyAst(body)) {
      collectException("missing for loop body");
    }

    return syntax::ForStmt{condition, body};
  }

  auto Parser::parseMatchStmt() -> ASTNode {
    if (!match(EToken::Match)) {
      return {};
    }

    bool hasArguments = true;
    auto matchedExpr = parseTupleExpr();
    if (isEmptyAst(matchedExpr)) {
      matchedExpr = parseIdentifier();
    }
    if (isEmptyAst(matchedExpr)) {
      hasArguments = false;
    }

    if (!match(EToken::LeftBrace)) {
      collectException("missing {");
    }

    Vec<ASTNode> cases;
    ASTNode defaultBody = {};
    while (!match(EToken::RightBrace)) {
      ASTNode value = {}, condition = {};

      if (!hasArguments) {
        if (match(EToken::When)) {
          collectException("'when' can be ignored since match has zero arguments");
        }

        condition = parseExpr();
      } else {
        value = parseTupleExpr();
        if (isEmptyAst(value)) {
          value = parseExpr();
        }
        if (isEmptyAst(value)) {
          collectException("missing matching expression");
        }
      }

      if (hasArguments && match(EToken::When)) {
        condition = parseExpr();
      }

      if (!match(EToken::EqualGreater)) {
        collectException("missing =>");
      }

      // todo: allow one-line statements
      const auto body = parseBlockStmt();

      if (!hasArguments && syntax::matchAstType<syntax::Identifier>(condition)) {
        if (const auto condExpr = syntax::astCast<syntax::Identifier>(condition);
          condExpr.isAnonymous()) {
          defaultBody = body;

          // optional comma
          match(EToken::Comma);
          break;
        }
      } else if (isEmptyAst(condition) && syntax::matchAstType<syntax::Identifier>(value)) {
        if (const auto valueExpr = syntax::astCast<syntax::Identifier>(value);
          valueExpr.isAnonymous()) {
          defaultBody = body;

          // optional comma
          match(EToken::Comma);
          break;
        }
      }

      if (!match(EToken::Comma)) {
        collectException("missing , at the end of matching case");
      }

      cases.emplace_back(syntax::MatchStmtCase{value, condition, body});
    }

    if (isEmptyAst(defaultBody)) {
      collectException("missing default case");
    } else if (!match(EToken::RightBrace)) {
      collectException("missing } in match statement");
    }

    return syntax::MatchStmt{matchedExpr, defaultBody, cases};
  }

  auto Parser::parseBlockStmt() -> ASTNode {
    if (!match(EToken::LeftBrace)) {
      return {};
    }

    Vec<ASTNode> stmts;
    auto stmt = parseStmt();
    while (!isEmptyAst(stmt)) {
      stmts.push_back(stmt);
      stmt = parseStmt();
    }

    if (!match(EToken::RightBrace)) {
      collectException("missing '}'");

      // todo
      if (stmts.empty()) {
        return {};
      }
    }

    // optional ';'
    match(EToken::Semicolon);

    return syntax::BlockStmt{stmts};
  }

  auto Parser::parseExprPrefixStmt() -> ASTNode {
    markRevertPoint();

    const auto lhs = parseExpr();
    if (isEmptyAst(lhs)) {
      toRevertPoint();
      return {};
    }

    if (match(EToken::Semicolon)) {
      // expr statement
      return syntax::ExprStmt{lhs};
    }

    if (match(EToken::EqualGreater)) {
      if (isEmptyAst(lhs)) {
        collectException("missing expression before =>");
      }

      auto body = parseStmt();
      if (isEmptyAst(body)) {
        collectException("missing statement after =>");
      }

      return syntax::ConditionalStmt{lhs, body};
    }

    if (!match(EToken::Equal, EToken::PlusEqual, EToken::MinusEqual, EToken::StarEqual,
               EToken::FwdSlashEqual, EToken::PercentEqual, EToken::Star2Equal,
               EToken::AmpersandEqual, EToken::BarEqual, EToken::HatEqual, EToken::Less2Equal,
               EToken::Greater2Equal)) {
      toRevertPoint();
      return {};
    }

    const String op = current().string();

    const auto rhs = parseExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of assignment statement");
    }

    if (!match(EToken::Semicolon)) {
      collectException("missing ; at the end of assignment statement");
    }

    return syntax::AssignStmt{lhs, rhs, op};
  }

  auto Parser::parseLetStmt() -> ASTNode {
    if (!match(EToken::Let)) {
      return {};
    }

    auto decl = parseTupleDecl();
    if (isEmptyAst(decl)) {
      decl = parseIdentifierDecl();
    }
    if (isEmptyAst(decl)) {
      collectException("missing local declaration");
    }

    if (!match(EToken::Equal)) {
      if (!match(EToken::Semicolon)) {
        collectException("missing ; at the end of let statement");
      }

      return syntax::LetStmt{decl, {}};
    }

    auto init = parseExpr();
    if (isEmptyAst(decl)) {
      collectException("missing initialization for local declaration");
    }

    if (!match(EToken::Semicolon)) {
      collectException("missing ; at the end of let statement");
    }

    return syntax::LetStmt{decl, init};
  }

  auto Parser::parseReturnStmt() -> ASTNode {
    if (!match(EToken::Return)) {
      return {};
    }

    const auto expr = parseExpr();

    if (!match(EToken::Semicolon)) {
      collectException("missing ; at the end of return statement");
    }

    return syntax::ReturnStmt{expr};
  }

  auto Parser::parseExpr() -> ASTNode {
    return parseTernaryExpr();
  }

  auto Parser::parseTernaryExpr() -> ASTNode {
    const auto cond = parseSequenceExpr();

    if (!match(EToken::QMark)) {
      return cond;
    }

    const auto ifTrue = parseSequenceExpr();
    if (isEmptyAst(ifTrue)) {
      collectException("missing if-true expression of conditional ternary expression");
    }

    if (!match(EToken::Colon)) {
      collectException("missing : of conditional ternary expression");
    }

    const auto ifFalse = parseSequenceExpr();
    if (isEmptyAst(ifFalse)) {
      collectException("missing if-false expression of conditional ternary expression");
    }

    return syntax::TernaryExpr{cond, ifTrue, ifFalse, "?", ":"};
  }

  auto Parser::parseSequenceExpr() -> ASTNode {
    const auto from = parseNullCoalescingExpr();

    if (!match(EToken::Dot2)) {
      return from;
    }

    const auto to = parseNullCoalescingExpr();
    if (isEmptyAst(to)) {
      collectException("missing to expression of sequence expression");
    }

    if (match(EToken::By, EToken::At)) {
      const auto by = parseNullCoalescingExpr();
      if (isEmptyAst(by)) {
        collectException("missing by expression of sequence expression");
      }

      return syntax::TernaryExpr{from, to, by, "..", "@"};
    }

    return syntax::TernaryExpr{from, to, syntax::IntegerLiteral{1}, "..", "@"};
  }

  auto Parser::parseNullCoalescingExpr() -> ASTNode {
    const auto lhs = parseLogicalOrExpr();

    if (!match(EToken::QMark2)) {
      return lhs;
    }

    const auto rhs = parseLogicalOrExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of null coalescing expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "??"};
  }

  auto Parser::parseLogicalOrExpr() -> ASTNode {
    const auto lhs = parseLogicalAndExpr();

    if (!match(EToken::Bar2)) {
      return lhs;
    }

    const auto rhs = parseLogicalAndExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of logical or expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "||"};
  }

  auto Parser::parseLogicalAndExpr() -> ASTNode {
    const auto lhs = parseInclusiveOrExpr();

    if (!match(EToken::Ampersand2)) {
      return lhs;
    }

    const auto rhs = parseInclusiveOrExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of logical and expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "&&"};
  }

  auto Parser::parseInclusiveOrExpr() -> ASTNode {
    const auto lhs = parseExclusiveOrExpr();

    if (!match(EToken::Bar)) {
      return lhs;
    }

    const auto rhs = parseExclusiveOrExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of inclusive or expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "|"};
  }

  auto Parser::parseExclusiveOrExpr() -> ASTNode {
    const auto lhs = parseAndExpr();

    if (!match(EToken::Hat)) {
      return lhs;
    }

    const auto rhs = parseAndExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of exclusive or expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "^"};
  }

  auto Parser::parseAndExpr() -> ASTNode {
    const auto lhs = parseEqualityExpr();

    if (!match(EToken::Ampersand)) {
      return lhs;
    }

    const auto rhs = parseEqualityExpr();
    if (isEmptyAst(rhs)) {
      collectException("missing rhs expression of and expression");
    }

    return syntax::BinaryExpr{lhs, rhs, "&"};
  }

  auto Parser::parseEqualityExpr() -> ASTNode {
    const auto lhs = parseRelationalExpr();

    if (match(EToken::Equal2, EToken::ExclaimEqual)) {
      const auto op = current().string();

      const auto rhs = parseRelationalExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of equality expression");
      }

      return syntax::BinaryExpr{lhs, rhs, op};
    }

    return lhs;
  }

  auto Parser::parseRelationalExpr() -> ASTNode {
    const auto lhs = parseShiftExpr();

    if (match(EToken::Less, EToken::LessEqual, EToken::Greater, EToken::GreaterEqual)) {
      const auto op = current().string();

      const auto rhs = parseShiftExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of relational expression");
      }

      return syntax::BinaryExpr{lhs, rhs, op};
    }

    return lhs;
  }

  auto Parser::parseShiftExpr() -> ASTNode {
    const auto lhs = parseAdditiveExpr();

    if (match(EToken::Less2, EToken::Greater2)) {
      const auto op = current().string();

      const auto rhs = parseAdditiveExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of shift expression");
      }

      return syntax::BinaryExpr{lhs, rhs, op};
    }

    return lhs;
  }

  auto Parser::parseAdditiveExpr() -> ASTNode {
    const auto lhs = parseMultiplicativeExpr();

    if (match(EToken::Plus, EToken::Minus)) {
      const auto op = current().string();

      const auto rhs = parseMultiplicativeExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of additive expression");
      }

      return syntax::BinaryExpr{lhs, rhs, op};
    }

    return lhs;
  }

  auto Parser::parseMultiplicativeExpr() -> ASTNode {
    const auto lhs = parseExponentialExpr();

    if (match(EToken::Star, EToken::FwdSlash, EToken::Percent)) {
      const auto op = current().string();

      const auto rhs = parseExponentialExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of multiplicative expression");
      }

      return syntax::BinaryExpr{lhs, rhs, op};
    }

    return lhs;
  }

  auto Parser::parseExponentialExpr() -> ASTNode {
    const auto lhs = parsePrefixUnaryExpr();

    if (match(EToken::Star2)) {
      const auto rhs = parsePrefixUnaryExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of exponential expression");
      }

      return syntax::BinaryExpr{lhs, rhs, "**"};
    }

    return lhs;
  }

  auto Parser::parsePrefixUnaryExpr() -> ASTNode {
    String op;
    if (match(
      EToken::Exclaim, EToken::Tilde, EToken::Plus,
      EToken::Minus, EToken::Hash, EToken::Dot3
    )) {
      op = current().string();
    }

    const auto operand = parsePostfixExpr();
    return op.empty() ? operand : syntax::UnaryExpr(operand, op);
  }

  auto Parser::parsePostfixExpr() -> ASTNode {
    auto lhs = parsePrimaryExpr();

    if (const auto tuple = parseTupleExpr(); !isEmptyAst(tuple)) {
      return syntax::FunctionCallExpr(lhs, tuple);
    }

    if (match(EToken::BarGreater)) {
      const auto rhs = parsePostfixExpr();
      if (isEmptyAst(rhs)) {
        collectException("missing rhs expression of pipe expression");
      }

      const auto funCallExpr = syntax::FunctionCallExpr::fromPipeExpr(lhs, rhs);
      if (!funCallExpr.has_value()) {
        collectException(
          "rhs expression of pipe expression must be either function application, type identifier or identifier"
        );
      }
      return funCallExpr.value();
    }

    if (match(EToken::LeftBracket)) {
      const auto subscript = parseExpr();
      if (isEmptyAst(subscript)) {
        collectException("missing subscript expression of subscripting expression");
      }

      if (!match(EToken::RightBracket)) {
        collectException("missing ] of subscripting expression");
      }

      return syntax::SubScriptingExpr{lhs, subscript};
    }

    if (match(EToken::Dot)) {
      auto field = parseIdentifier();
      if (isEmptyAst(field)) {
        field = parseTypeIdentifier();
      }
      if (isEmptyAst(field)) {
        collectException("missing field expression of access expression");
      }
      return syntax::AccessExpr{lhs, field};
    }

    return lhs;
  }

  auto Parser::parsePrimaryExpr() -> ASTNode {
    if (match(EToken::IntegerLiteral)) {
      // todo: stoll exceptions
      return syntax::IntegerLiteral{std::stoll(current().string())};
    }

    if (match(EToken::FloatLiteral)) {
      // todo: stod exceptions
      return syntax::FloatLiteral{std::stod(current().string())};
    }

    if (match(EToken::True, EToken::False)) {
      return syntax::BooleanLiteral{current().type() == EToken::True};
    }

    if (match(EToken::StringLiteral)) {
      // todo: placeholders
      return syntax::StringLiteral{current().string()};
    }

    if (match(EToken::LeftBracket)) {
      Vec<ASTNode> elements;
      do {
        ASTNode element = parseExpr();
        if (isEmptyAst(element)) {
          if (peekNext().type() == EToken::Comma) {
            collectException("missing array element");
          } else {
            break;
          }
        }
        elements.emplace_back(element);
      } while (match(EToken::Comma));

      if (!match(EToken::RightBracket)) {
        collectException("missing ] of array expression");
      }

      return syntax::ArrayExpr{elements};
    }

    if (const auto identifier = parseIdentifier(); !isEmptyAst(identifier)) {
      return identifier;
    }

    // todo:
    if (const auto typeId = parseTypeIdentifier(); !isEmptyAst(typeId)) {
      return typeId;
    }

    // todo: paren
    if (const auto tuple = parseTupleExpr(); !isEmptyAst(tuple)) {
      return tuple;
    }

    enterContext(Context::Lambda);
    if (const auto lambda = parseFunctionDef(); !isEmptyAst(lambda)) {
      return lambda;
    }
    exitContext();

    // todo: types

    return {};
  }

  auto Parser::parseTupleExpr() -> ASTNode {
    if (!match(EToken::LeftParen)) {
      return {};
    }

    // ()
    if (match(EToken::RightParen)) {
      return syntax::TupleExpr{{}};
    }

    Vec<ASTNode> elements;
    do {
      ASTNode element = parseExpr();
      if (isEmptyAst(element)) {
        if (peekNext().type() == EToken::Comma) {
          collectException("missing tuple element");
        } else {
          break;
        }
      }
      elements.emplace_back(element);
    } while (match(EToken::Comma));

    if (!match(EToken::RightParen)) {
      collectException("missing )");
    }

    return syntax::TupleExpr{std::move(elements)};
  }
}
