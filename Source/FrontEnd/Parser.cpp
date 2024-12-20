#include "Parser.hpp"

#include <utility>

using tl::syntax::ASTNode;

namespace tl::fe {
  auto Parser::current() const -> CRef<Token> {
    if (m_currentToken == nullptr) {
      throw std::runtime_error("Parser: m_currentToken == nullptr");
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

  auto Parser::createError(ToyLangException &&e) const -> void {
    m_eCollector->add(std::move(e));
  }

  auto Parser::operator()(
    ExpceptionCollector &eCollector, String filepath, Tokens tokens
  ) -> syntax::TranslationUnit {
    m_filepath = std::move(filepath);
    m_tokenIt = tokens.begin();
    m_tokenItEnd = tokens.end();
    m_eCollector = &eCollector;

    if (m_tokenIt == m_tokenItEnd) {
      createError(
        ParserExpception(filepath, "module declaration not found")
      );
    }

    return parseTranslationUnit();
  }

  auto Parser::parseTranslationUnit() -> syntax::TranslationUnit {
    std::vector<ASTNode> definitions;

    while (m_tokenIt != m_tokenItEnd) {
      ASTNode definition;

      if (match(EToken::Module)) {
        definition = parseModuleDecl();
      } else if (match(EToken::Import)) {
        definition = parseImportDecl();
      } else if (match(EToken::Type)) {
        definition = parseTypeDecl();
      } else {
        if (match(EToken::Export, EToken::Internal, EToken::Local)) {
          switch (peekPrev().type()) {
            case EToken::Export: {
              currentStorage = syntax::Storage::Export;
              break;
            }
            case EToken::Internal: {
              currentStorage = syntax::Storage::Internal;
              break;
            }
            default: {
              currentStorage = syntax::Storage::Local;
              break;
            }
          }
        }

        if (match(EToken::Fn)) {
          definition = parseFunctionDef();
        } else if (match(EToken::Class)) {
          classState = true;
          definition = parseClassDef();
          classState = false;
        } else if (match(EToken::Interface)) {
          interfaceState = true;
          definition = parseInterfaceDef();
          interfaceState = false;
        }
      }

      currentStorage = syntax::Storage::Internal;
      currentAccess = syntax::Access::Private;

      // if (isEmpty(definition)) {
      //   throw;
      // }

      definitions.emplace_back(definition);
    }

    return syntax::TranslationUnit(definitions);
  }

  auto Parser::parseModuleDecl() -> ASTNode {
    if (const auto identifier = parseIdentifier();
      !isEmpty(identifier)) {
      return syntax::ModuleDecl{identifier};
    }

    throw ParserExpception(m_filepath, current(), "module path must be specified");
  }

  auto Parser::parseImportDecl() -> ASTNode {
    if (const auto identifier = parseIdentifier();
      !isEmpty(identifier)) {
      return syntax::ImportDecl{identifier};
    }

    throw ParserExpception(m_filepath, current(), "module path must be specified");
  }

  auto Parser::parseTypeDecl() -> ASTNode {
    return {};
  }

  auto Parser::parseFunctionDef() -> ASTNode {
    return {};
  }

  auto Parser::parseClassDef() -> ASTNode {
    return {};
  }

  auto Parser::parseInterfaceDef() -> ASTNode {
    return {};
  }

  auto Parser::parseIdentifier() -> ASTNode {
    Vec<String> path;

    while (match(EToken::Identifier)) {
      path.push_back(current().string());
      if (!match(EToken::Colon2)) {
        break;
      }
    }

    if (!match(EToken::Semicolon)) {
      throw ParserExpception(m_filepath, current(), "missing ; at the end of identifier");
    }

    return syntax::Identifier{path};
  }
}
