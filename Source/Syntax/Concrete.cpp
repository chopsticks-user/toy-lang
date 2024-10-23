#include "Concrete.hpp"

#include <utility>
#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  TranslationUnit::TranslationUnit(Vec<ASTNode> definitions)
    : ASTNodeBase(std::move(definitions)) {
  }

  Function::Function(
    ASTNode fIdentifier, ASTNode fPrototype,
    ASTNode fBody,
    String storageSpecifier, const bool pure
  )
    : ASTNodeBase({fIdentifier, fPrototype, fBody}),
      m_pure(pure), m_storage(storageSpecifier.empty() ? "export" : std::move(storageSpecifier)) {
  }

  Clazz::Clazz(String visibility, Vec<ASTNode> parents, CRef<ASTNode> body)
    : ASTNodeBase({
        [&]() {
          // todo: move args
          auto v = std::vector{body};
          v.insert(v.end(), parents.begin(), parents.end());
          return v;
        }()
      }), m_visibility(std::move(visibility)) {
  }

  FunctionPrototype::FunctionPrototype(
    CRef<ASTNode> returnTypeExpr,
    Vec<ASTNode> parameterDecls
  ) : ASTNodeBase({
    [&] {
      // todo: move args
      auto v = std::vector{returnTypeExpr};
      v.insert(v.end(), parameterDecls.begin(), parameterDecls.end());
      return v;
    }()
  }) {
  }
}
