#include "Parser.hpp"

#include <utility>

using tl::syntax::ASTNode;

namespace tl::fe {
  auto Parser::current() const -> CRef<Token> {
    if (m_currentToken == nullptr) {
      throw InternalException(m_filepath, "Parser: m_currentToken == nullptr");
    }
    return *m_currentToken;
  }

  auto Parser::revert() -> void {
    --m_tokenIt;
    m_currentToken = &(*m_tokenIt);
  }

  auto Parser::advance() -> void {
    ++m_tokenIt;
    m_currentToken = &(*m_tokenIt);
  }

  auto Parser::match(std::same_as<EToken> auto... expected) -> bool {
    if (m_tokenIt == m_tokenItEnd) {
      return false;
    }

    if (!((m_tokenIt->type() == expected) || ...)) {
      return false;
    }

    m_currentToken = &(*m_tokenIt);
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

  auto Parser::setStorage(const syntax::Storage storage) -> void {
    m_currentStorage = storage;
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

    if (match(EToken::Module)) {
      definitions.push_back(parseModuleDecl());
    } else {
      throw ParserExpception(m_filepath, current(), "module declaration must be specified");
    }

    while (match(EToken::Import)) {
      definitions.push_back(parseImportDecl());
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

      if (match(EToken::Fn)) {
        definition = parseFunctionDef();
      } else if (match(EToken::Type)) {
        definition = parseTypeDecl();
      } else if (match(EToken::Class)) {
        definition = parseClassDef();
      } else if (match(EToken::Concept)) {
        definition = parseConceptDef();
      } else {
        throw ParserExpception(m_filepath, current(), "unknown statement");
      }

      setStorage();
      definitions.emplace_back(definition);
    }

    return syntax::TranslationUnit(definitions);
  }

  auto Parser::parseModuleDecl() -> ASTNode {
    if (const auto identifier = parseIdentifier();
      !isEmptyAst(identifier)) {
      if (!match(EToken::Semicolon)) {
        collectException(
          ParserExpception(m_filepath, current(), "missing ; at the end of module declaration")
        );
      }

      return syntax::ModuleDecl{identifier};
    }

    collectException(
      ParserExpception(m_filepath, current(), "module path must be specified")
    );
    return {};
  }

  auto Parser::parseImportDecl() -> ASTNode {
    if (const auto identifier = parseIdentifier();
      !isEmptyAst(identifier)) {
      if (!match(EToken::Semicolon)) {
        collectException(
          ParserExpception(m_filepath, current(), "missing ; at the end of import declaration")
        );
      }

      return syntax::ImportDecl{identifier};
    }

    collectException(
      ParserExpception(m_filepath, current(), "module path must be specified")
    );
    return {};
  }

  auto Parser::parseTypeDecl() -> ASTNode {
    const auto typeId = parseIdentifier();
    if (isEmptyAst(typeId)) {
      collectException(
        ParserExpception(m_filepath, current(), "missing lhs type identifier for type declaration")
      );
    }

    if (syntax::astCast<syntax::Identifier>(typeId).imported()) {
      collectException(
        ParserExpception(m_filepath, current(), "imported type cannot be re-defined")
      );
    }

    if (!match(EToken::Equal)) {
      collectException(
        ParserExpception(m_filepath, current(), "missing = for type declaration")
      );
    }

    const auto typeExpr = parseTypeExpr();
    if (isEmptyAst(typeExpr)) {
      collectException(
        ParserExpception(m_filepath, current(), "missing rhs type expression for type declaration")
      );
    }

    if (!match(EToken::Semicolon)) {
      collectException(
        ParserExpception(m_filepath, current(), "missing ; at the end of type declaration")
      );
    }

    return syntax::TypeDecl{typeId, typeExpr};
  }

  auto Parser::parseFunctionDef() -> ASTNode {
    return {};
  }

  auto Parser::parseClassDef() -> ASTNode {
    return {};
  }

  auto Parser::parseConceptDef() -> ASTNode {
    return {};
  }

  auto Parser::parseIdentifier() -> ASTNode {
    Vec<String> path;

    while (match(EToken::Identifier, EToken::UserDefinedType, EToken::FundamentalType)) {
      const auto id = current();

      path.push_back(id.string());
      if (!match(EToken::Colon2)) {
        break;
      }

      if (id.type() != EToken::Identifier) {
        collectException(
          ParserExpception(m_filepath, current(), "path to imported name cannot be type")
        );
        break;
      }
    }

    // identifier
    return syntax::Identifier{path};
  }

  auto Parser::parseTypeExpr() -> ASTNode {
    Vec<ASTNode> types;

    do {
      if (auto type = parseIdentifier();
        !isEmptyAst(type) && syntax::astCast<syntax::Identifier>(type).isType()) {
        types.emplace_back(type);
      } else {
        collectException(
          ParserExpception(m_filepath, current(), "invalid type identifier")
        );
      }
    } while (match(EToken::Bar));

    return syntax::TypeExpr{types};
  }
}
