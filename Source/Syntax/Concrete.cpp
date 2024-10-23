#include "Concrete.hpp"

#include <utility>
#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  TranslationUnit::TranslationUnit(Vec<VNode> definitions)
    : Node(std::move(definitions)) {
  }

  Function::Function(
    VNode fIdentifier, VNode fPrototype,
    VNode fBody,
    String storageSpecifier, const bool pure
  )
    : Node({fIdentifier, fPrototype, fBody}),
      m_pure(pure), m_storage(storageSpecifier.empty() ? "export" : std::move(storageSpecifier)) {
  }

  Clazz::Clazz(String visibility, Vec<VNode> parents, CRef<VNode> body)
    : Node({
        [&]() {
          // todo: move args
          auto v = std::vector{body};
          v.insert(v.end(), parents.begin(), parents.end());
          return v;
        }()
      }), m_visibility(std::move(visibility)) {
  }

  FunctionPrototype::FunctionPrototype(
    CRef<VNode> returnTypeExpr,
    Vec<VNode> parameterDecls
  ) : Node({
    [&] {
      // todo: move args
      auto v = std::vector{returnTypeExpr};
      v.insert(v.end(), parameterDecls.begin(), parameterDecls.end());
      return v;
    }()
  }) {
  }
}
