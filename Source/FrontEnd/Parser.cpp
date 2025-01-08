#include "Parser.hpp"

#include <utility>

using tl::syntax::ASTNode;

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
    ++m_tokenIt;
    return *(m_tokenIt--);
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

  auto Parser::parseFunctionDef() -> ASTNode {
    const auto prototype = parseFunctionPrototype();

    if (isEmptyAst(prototype)) {
      return {};
    }

    return {};
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
      } else {
        collectException("invalid type identifier");
      }
    } while (match(EToken::Bar));

    if (types.empty()) {
      return {};
    }

    return syntax::TypeExpr{types};
  }

  auto Parser::parseFunctionPrototype() -> ASTNode {
    if (!match(EToken::Fn)) {
      return {};
    }

    auto identifier = parseIdentifier();

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

    auto params = parseTupleDecl();
    if (isEmptyAst(params)) {
      params = parseIdentifierDecl();
    }
    if (isEmptyAst(params)) {
      params = parseTypeExpr();
    }
    if (isEmptyAst(params)) {
      collectException("missing function parameters");
    }

    if (!match(EToken::MinusGreater)) {
      collectException("missing -> after function parameters");
    }

    auto returns = parseTypeExpr();
    if (isEmptyAst(returns)) {
      returns = parseTupleDecl();
    }
    if (isEmptyAst(returns)) {
      returns = parseIdentifierDecl();
    }
    if (isEmptyAst(returns)) {
      collectException("missing function returns");
    }

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

      if (isEmptyAst(decl)) {
        collectException("invalid identifier declaration");
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
    if (const auto forStmt = parseForStmt(); isEmptyAst(forStmt)) {
      return forStmt;
    }

    if (const auto matchStmt = parseMatchStmt(); isEmptyAst(matchStmt)) {
      return matchStmt;
    }

    if (const auto blockStmt = parseBlockStmt(); isEmptyAst(blockStmt)) {
      return blockStmt;
    }

    if (const auto letStmt = parseLetStmt(); isEmptyAst(letStmt)) {
      return letStmt;
    }

    if (const auto returnStmt = parseReturnStmt(); isEmptyAst(returnStmt)) {
      return returnStmt;
    }

    // must be last
    if (const auto stmt = parseAssignOrExprStmt(); isEmptyAst(stmt)) {
      return stmt;
    }

    // collectException("unknown statement");
    return {};
  }

  auto Parser::parseForStmt() -> ASTNode {
    if (!match(EToken::For)) {
      return {};
    }

    const auto iterator = parseTupleDecl();
    if (isEmptyAst(iterator)) {
      collectException("missing iterator declaration");
    }

    if (!match(EToken::In)) {
      collectException("missing 'in'");
    }

    const auto iterable = parseExpr();
    if (isEmptyAst(iterable)) {
      collectException("missing iterable expression");
    }

    const auto body = parseBlockStmt();
    if (isEmptyAst(body)) {
      collectException("missing for loop body");
    }

    return syntax::ForStmt{iterator, iterable, body};
  }

  auto Parser::parseMatchStmt() -> ASTNode {
    if (!match(EToken::Match)) {
      return {};
    }

    u64 idCount = 0;
    bool isTuple = true;
    auto matchedExpr = parseTupleExpr();
    if (isEmptyAst(matchedExpr)) {
      matchedExpr = parseIdentifier();
    } else {
      idCount = syntax::astCast<syntax::TupleExpr>(matchedExpr).nChildren();
    }
    if (isEmptyAst(matchedExpr)) {
      matchedExpr = parseIdentifier();
      isTuple = false;
    } else {
      idCount = 1;
    }

    if (!match(EToken::LeftBrace)) {
      collectException("missing {");
    }

    Vec<ASTNode> cases;
    while (peekNext().type() != EToken::AnnonymousIdentifier ||
           peekNext().type() != EToken::RightBrace) {
      ASTNode value = {}, condition = {};

      if (idCount == 0) {
        if (match(EToken::If)) {
          collectException("'if' can be ignored since match has zero arguments");
        }
        condition = parseExpr();
      } else {
        if (isTuple) {
          value = parseTupleExpr();
          if (!isEmptyAst(value)) {
            if (const auto n = syntax::astCast<syntax::TupleExpr>(value).nChildren();
              n != idCount) {
              collectException(
                "tuple of "s + std::to_string(idCount) + " elements cannot be matched with tuple of "
                + std::to_string(n) + " elements"
              );
            }
          } else {
            collectException(
              "matching expressions must be tuples as matched expression is a tuple"
            );
            value = parseExpr();
            if (isEmptyAst(value)) {
              collectException("missing matching expression");
            }
          }
        } else {
          value = parseExpr();
          if (isEmptyAst(value)) {
            collectException("missing matching expression");
          }

          if (syntax::matchAstType<syntax::TupleExpr>(value)) {
            collectException(
              "matching expressions cannot be tuples as matched expression is not a tuple"
            );
          }
        }
      }

      if (!match(EToken::EqualGreater)) {
        collectException("missing =>");
      }

      // todo: allow one-line statements
      const auto body = parseBlockStmt();

      if (!match(EToken::Comma)) {
        collectException("missing , at the end of matching case");
      }

      cases.emplace_back(syntax::MatchStmtCase{value, condition, body});
    }

    ASTNode defaultBody = {};
    if (match(EToken::AnnonymousIdentifier)) {
      if (!match(EToken::EqualGreater)) {
        collectException("missing =>");
      }
      defaultBody = parseBlockStmt();
    } else {
      collectException("missing default case");
    }

    if (!match(EToken::RightBrace)) {
      collectException("missing } in match statement");
    }

    return syntax::MatchStmt{matchedExpr, defaultBody, cases};
  }

  auto Parser::parseBlockStmt() -> ASTNode {
    if (!match(EToken::RightBrace)) {
      return {};
    }

    Vec<ASTNode> stmts;
    auto stmt = parseStmt();
    while (isEmptyAst(stmt)) {
      stmts.push_back(stmt);
      stmt = parseStmt();
    }

    if (!match(EToken::LeftBrace)) {
      collectException("missing '}'");

      // todo
      if (stmts.empty()) {
        return {};
      }
    }

    return syntax::BlockStmt{stmts};
  }

  auto Parser::parseAssignOrExprStmt() -> ASTNode {
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

    // String op;
    // if (match(
    //   EToken::Equal, EToken::StarEqual, EToken::AmpersandEqual, EToken::BarEqual,
    //   EToken::FwdSlashEqual, EToken::PercentEqual, EToken::PlusEqual, EToken::MinusEqual,
    //   EToken::HatEqual, EToken::Greater2Equal, EToken::Less2Equal, EToken::Star2Equal
    // )) {
    //   op = current().string();
    // } else {
    //   toRevertPoint();
    //   return {};
    // }

    String op;
    if (const auto str = current().string(); syntax::assignmentOps.contains(op)) {
      op = str;
    } else {
      toRevertPoint();
      return {};
    }

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
    if (isEmptyAst(expr)) {
      collectException("missing returned expression");
    }

    if (!match(EToken::Semicolon)) {
      collectException("missing ; at the end of return statement");
    }

    return syntax::ReturnStmt{expr};
  }

  auto Parser::parseExpr() -> ASTNode {
    return {};
  }

  auto Parser::parseTupleExpr() -> ASTNode {
  }
}
