#include "Definitions.hpp"

#include <utility>

#include "Expressions.hpp"
#include "Statements.hpp"

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

  IdentifierDecl::IdentifierDecl(CRef<ASTNode> identifier, CRef<ASTNode> typeExpr)
    : ASTNodeBase({identifier, typeExpr}) {
  }

  TupleDecl::TupleDecl(Vec<ASTNode> idDecls)
    : ASTNodeBase(std::move(idDecls)) {
  }

  ParameterDecl::ParameterDecl(Vec<ASTNode> idDecls)
    : ASTNodeBase(std::move(idDecls)) {
  }

  ReturnDecl::ReturnDecl(Vec<ASTNode> typeExprs)
    : ASTNodeBase(std::move(typeExprs)) {
  }

  FunctionPrototype::FunctionPrototype(
    const FnType fType, ASTNode fIdentifier, ASTNode fParamDecls, ASTNode fReturnDecls
  ): ASTNodeBase({fIdentifier, fParamDecls, fReturnDecls}),
     m_type(fType) {
  }

  FunctionDef::FunctionDef(
    const Storage fStorage, ASTNode fPrototype, ASTNode fBody
  ): ASTNodeBase({fPrototype, fBody}), m_storage(fStorage) {
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
}
