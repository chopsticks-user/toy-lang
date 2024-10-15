#include "Concrete.hpp"

#include <utility>
#include "Expressions.hpp"
#include "Statements.hpp"

namespace tl::syntax {
  TranslationUnit::TranslationUnit(std::vector<VNode> definitions)
    : Node(std::move(definitions)) {
  }

  Function::Function(
    std::optional<VNode> fIdentifier, std::optional<VNode> fPrototype,
    std::optional<VNode> fBody,
    std::string storageSpecifier, bool pure
  )
    : Node({fIdentifier.value(), fPrototype.value(), fBody.value()}),
      m_pure(pure), m_storage(std::move(storageSpecifier)) {
  }

  Clazz::Clazz(std::string visibility, std::vector<VNode> parents, std::optional<VNode> body)
    : Node({
        [&]() {
          // todo: move args
          auto v = std::vector{body.value()};
          v.insert(v.end(), parents.begin(), parents.end());
          return v;
        }()
      }), m_visibility(std::move(visibility)) {
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
