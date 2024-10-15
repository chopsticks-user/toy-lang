#include "Concrete.hpp"

#include <utility>
#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  TranslationUnit::TranslationUnit(std::vector<VNode> definitions)
    : Node(std::move(definitions)) {
  }

  Function::Function(
    VNode fIdentifier, VNode fPrototype,
    VNode fBody,
    std::string storageSpecifier, bool pure
  )
    : Node({fIdentifier, fPrototype, fBody}),
      m_pure(pure), m_storage(std::move(storageSpecifier)) {
  }

  Clazz::Clazz(std::string visibility, std::vector<VNode> parents, VNode body)
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
    VNode returnTypeExpr,
    std::vector<VNode> parameterDecls
  ) : Node({
    [&]() {
      // todo: move args
      auto v = std::vector{returnTypeExpr};
      v.insert(v.end(), parameterDecls.begin(), parameterDecls.end());
      return v;
    }()
  }) {
  }
}
