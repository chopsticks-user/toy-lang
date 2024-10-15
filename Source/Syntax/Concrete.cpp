#include "Concrete.hpp"
#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  Function::Function(
    std::optional<VNode> fIdentifier, std::optional<VNode> fPrototype,
    std::optional<VNode> fBody,
    std::string storageSpecifier, bool pure
  )
    : Node({fIdentifier.value(), fPrototype.value(), fBody.value()}),
      m_pure(pure), m_storage(std::move(storageSpecifier)) {
  }

  FunctionPrototype::FunctionPrototype(
    std::optional<VNode> returnTypeExpr,
    std::vector<VNode> parameterDecls
  ) : Node({
    [&]() {
      // todo: move args
      auto v = std::vector{returnTypeExpr.value()};
      v.insert(v.end(), parameterDecls.begin(), parameterDecls.end());
      return v;
    }()
  }) {
  }
}
