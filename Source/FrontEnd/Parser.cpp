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

  auto Parser::setCurrentStorage(const syntax::Storage storage) -> void {
    m_currentStorage = storage;
  }

  auto Parser::currentStorage() const noexcept -> syntax::Storage {
    return m_currentStorage;
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
            setCurrentStorage(syntax::Storage::Export);
            break;
          }
          case EToken::Local: {
            setCurrentStorage(syntax::Storage::Local);
            break;
          }
          default: {
            setCurrentStorage(syntax::Storage::Internal);
            break;
          }
        }
      }

      definition = parseFunctionDef();

      if (isEmptyAst(definition)) {
        definition = parseTypeDecl();
      }

      if (isEmptyAst(definition)) {
        definition = parseClassDef();
      }

      if (isEmptyAst(definition)) {
        definition = parseConceptDef();
      }

      // todo: skip current line
      if (isEmptyAst(definition)) {
        throw ParserExpception(m_filepath, current(), "unknown global definition");
      }

      definitions.push_back(definition);
      setCurrentStorage();
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

    return syntax::TypeDecl{currentStorage(), typeId, typeExpr};
  }

  auto Parser::parseFunctionDef() -> ASTNode {
    if (!match(EToken::Fn)) {
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

    return {};
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
    const auto identifier = parseIdentifier();

    // todo
    if (isEmptyAst(identifier)) {
      collectException("missing function identifier");
    }

    if (!match(EToken::Colon)) {
      collectException("missing colon after function identifier");
    }

    const auto params = parseParameterDecl();
    if (isEmptyAst(params)) {
      collectException("missing function parameters");
    }

    if (!match(EToken::MinusGreater)) {
      collectException("missing -> after function parameters");
    }

    const auto returns = parseReturnDecl();
    if (isEmptyAst(returns)) {
      collectException("missing function returns");
    }

    return syntax::FunctionPrototype{
      syntax::FnType::Function, identifier, params, returns
    };
  }

  auto Parser::parseTupleDecl() -> ASTNode {
  }

  auto Parser::parseParameterDecl() -> ASTNode {
    // a: Int
    if (!match(EToken::LeftParen)) {
      return syntax::ParameterDecl{{parseIdentifierDecl()}};
    }

    // ()
    if (match(EToken::RightParen)) {
      return syntax::ParameterDecl{{}};
    }

    // (a: Int) or (a: Int, ...)
    Vec<ASTNode> idDecls;
    do {
      const auto idDecl = parseIdentifierDecl();
      idDecls.emplace_back(idDecl);
    } while (match(EToken::Comma));

    if (!match(EToken::RightParen)) {
      collectException("missing ')' for function parameter declaration");
    }

    return syntax::ParameterDecl{idDecls};
  }

  auto Parser::parseReturnDecl() -> ASTNode {
    // a: Int
    if (!match(EToken::LeftParen)) {
      return syntax::ReturnDecl{{parseTypeExpr()}};
    }

    // ()
    if (match(EToken::RightParen)) {
      return syntax::ReturnDecl{{}};
    }

    // (a: Int) or (a: Int, ...)
    Vec<ASTNode> typeExprs;
    do {
      const auto typeExpr = parseTypeExpr();
      typeExprs.emplace_back(typeExpr);
    } while (match(EToken::Comma));

    if (!match(EToken::RightParen)) {
      collectException("missing ')' for function parameter declaration");
    }

    return syntax::ReturnDecl{typeExprs};
  }

  auto Parser::parseIdentifierDecl() -> ASTNode {
    const auto identifier = parseIdentifier();

    if (match(EToken::Colon)) {
      const auto typeExpr = parseTypeExpr();

      if (isEmptyAst(typeExpr)) {
        collectException("missing ')' for function parameter declaration");
      }

      return syntax::IdentifierDecl{identifier, typeExpr};
    }

    return syntax::IdentifierDecl{identifier, {}};
  }
}
