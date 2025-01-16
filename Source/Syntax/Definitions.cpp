#include "Definitions.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"

#include <utility>

namespace tl::syntax {
  ModuleDecl::ModuleDecl(ASTNode nsIdentifier)
    : ASTNodeBase({nsIdentifier}) {
  }

  ImportDecl::ImportDecl(ASTNode nsIdentifier)
    : ASTNodeBase({nsIdentifier}) {
  }

  TypeDecl::TypeDecl(const Storage storage, ASTNode identifier, ASTNode typeExpr)
    : ASTNodeBase({identifier, typeExpr}), m_storage(storage) {
  }

  IdentifierDecl::IdentifierDecl(
    CRef<ASTNode> identifier, CRef<ASTNode> typeExpr, const bool isMutable
  ): ASTNodeBase({identifier, typeExpr}), m_mutable(isMutable) {
  }

  TupleDecl::TupleDecl(Vec<ASTNode> idDecls)
    : ASTNodeBase(std::move(idDecls)) {
  }

  FunctionPrototype::FunctionPrototype(
    const FnType fType, ASTNode fIdentifier, ASTNode fParamDecls, ASTNode fReturnDecls
  ): ASTNodeBase({fIdentifier, fParamDecls, fReturnDecls}),
     m_type(fType) {
  }

  auto FunctionPrototype::isAnonymous() const noexcept -> bool {
    return isEmptyAst(identifier());
  }

  FunctionDef::FunctionDef(
    const Storage fStorage, ASTNode fPrototype, ASTNode fBody
  ): ASTNodeBase({fPrototype, fBody}), m_storage(fStorage) {
  }

  auto FunctionDef::isLambda() const noexcept -> bool {
    return astCast<FunctionPrototype>(prototype()).isAnonymous();
  }

  ConceptDef::ConceptDef(const Storage storage, ASTNode identifier, Vec<ASTNode> requirements)
    : ASTNodeBase({
        [&]() {
          // todo: move args
          auto v = std::vector{identifier};
          v.insert(v.end(), requirements.begin(), requirements.end());
          return v;
        }()
      }), m_storage(storage) {
  }

  // ClassDef::ClassDef(String visibility, Vec<ASTNode> parents, CRef<ASTNode> body)
  //   : ASTNodeBase({
  //       [&]() {
  //         // todo: move args
  //         auto v = std::vector{body};
  //         v.insert(v.end(), parents.begin(), parents.end());
  //         return v;
  //       }()
  //     }), m_visibility(std::move(visibility)) {
  // }

  TranslationUnit::TranslationUnit(Vec<ASTNode> definitions)
    : ASTNodeBase(std::move(definitions)) {
  }

  TokenNode::TokenNode(fe::Token token)
    : ASTNodeBase({}), m_token(std::move(token)) {
  }
}
