#include "Definitions.hpp"

#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  ModuleDecl::ModuleDecl(ASTNode nsIdentifier)
    : ASTNodeBase({nsIdentifier}) {
  }

  ImportDecl::ImportDecl(ASTNode nsIdentifier)
    : ASTNodeBase({nsIdentifier}) {
  }

  TypeDecl::TypeDecl(ASTNode identifier, ASTNode typeExpr)
    : ASTNodeBase({identifier, typeExpr}) {
  }

  IdentifierDecl::IdentifierDecl(
    CRef<ASTNode> identifier,
    CRef<ASTNode> typeExpr,
    CRef<ASTNode> rhsExpr
  ) : ASTNodeBase(
    isEmptyAst(rhsExpr)
      ? Vec<ASTNode>{identifier, typeExpr}
      : Vec<ASTNode>{identifier, typeExpr, rhsExpr}
  ) {
  }

  TupleDecl::TupleDecl(Vec<ASTNode> varDecls)
    : ASTNodeBase(std::move(varDecls)) {
  }

  FunctionDef::FunctionDef(
    const Storage fStorage, ASTNode fIdentifier,
    ASTNode fParamDecls, ASTNode fReturnDecls, ASTNode fBody
  ): ASTNodeBase({fIdentifier, fParamDecls, fReturnDecls, fBody}),
     m_storage(fStorage) {
  }

  MethodDef::MethodDef(
    const Access mAccess, const Virtuality mVirtuality, ASTNode mIdentifier,
    ASTNode mParamDecls, ASTNode mReturnDecls, ASTNode mBody
  ) : ASTNodeBase({mIdentifier, mParamDecls, mReturnDecls, mBody}),
      m_access(mAccess), m_virtuality(mVirtuality) {
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
