#include "definition_nodes.hpp"
#include "expr_nodes.hpp"
#include "stmt_nodes.hpp"

namespace tlc::syntax {
  ModuleDecl::ModuleDecl(Node nsIdentifier)
    : NodeBase({nsIdentifier}) {
  }

  ImportDecl::ImportDecl(Node nsIdentifier)
    : NodeBase({nsIdentifier}) {
  }

  TypeDecl::TypeDecl(const Storage storage, Node identifier, Node typeExpr)
    : NodeBase({identifier, typeExpr}), m_storage(storage) {
  }

  IdentifierDecl::IdentifierDecl(
    const bool isMutable, Node const &identifier, Node const &typeExpr
  ): NodeBase({identifier, typeExpr}), m_mutable(isMutable) {
  }

  TupleDecl::TupleDecl(Vec<Node> idDecls)
    : NodeBase(std::move(idDecls)) {
  }

  GenericDecl::GenericDecl(Vec<Node> typeIds)
    : NodeBase(std::move(typeIds)) {
  }

  FunctionPrototype::FunctionPrototype(
    const FnType fType, Node fIdentifier, Node fParamDecls, Node fReturnDecls
  ): NodeBase({fIdentifier, fParamDecls, fReturnDecls}),
     m_type(fType) {
  }

  auto FunctionPrototype::isAnonymous() const noexcept -> bool {
    return isEmptyAst(identifier());
  }

  FunctionDef::FunctionDef(
    const Storage fStorage, Node fPrototype, Node fBody
  ): NodeBase({fPrototype, fBody}), m_storage(fStorage) {
  }

  auto FunctionDef::isLambda() const noexcept -> bool {
    return astCast<FunctionPrototype>(prototype()).isAnonymous();
  }

  ConceptDef::ConceptDef(const Storage storage, Node identifier, Vec<Node> requirements)
    : NodeBase({
        [&]() {
          // todo: move args
          auto v = std::vector{identifier};
          v.insert(v.end(), requirements.begin(), requirements.end());
          return v;
        }()
      }), m_storage(storage) {
  }

  // ClassDef::ClassDef(String visibility, Vec<ASTNode> parents, ASTNode const& body)
  //   : ASTNodeBase({
  //       [&]() {
  //         // todo: move args
  //         auto v = std::vector{body};
  //         v.insert(v.end(), parents.begin(), parents.end());
  //         return v;
  //       }()
  //     }), m_visibility(std::move(visibility)) {
  // }

  TranslationUnit::TranslationUnit(Vec<Node> definitions)
    : NodeBase(std::move(definitions)) {
  }

  TokenNode::TokenNode(token::Token token)
    : NodeBase({}), m_token(std::move(token)) {
  }
}
