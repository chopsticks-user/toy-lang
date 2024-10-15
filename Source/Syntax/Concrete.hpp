#ifndef  TOYLANG_SYNTAX_CONCRETE_HPP
#define  TOYLANG_SYNTAX_CONCRETE_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class TranslationUnit {
  };

  class Function final : public Node {
  public:
    Function(
      std::optional<VNode> fIdentifier, std::optional<VNode> fPrototype,
      std::optional<VNode> fBody,
      std::string storageSpecifier, bool pure
    );

    auto identifier() -> const VNode & {
      return childAt(0);
    }

    auto prototype() -> const VNode & {
      return childAt(1);
    }

    auto pure() const -> bool {
      return m_pure;
    }

    auto storage() const -> const std::string & {
      return m_storage;
    }

  private:
    bool m_pure;
    std::string m_storage;
  };

  class Clazz {
  };

  class FunctionPrototype final : public Node {
  public:
    FunctionPrototype(
      std::optional<VNode> returnTypeExpr,
      std::vector<VNode> parameterDecls
    );

    auto returnTypeExpr() -> const VNode & {
      return childAt(0);
    }
  };
}

#endif // TOYLANG_SYNTAX_CONCRETE_HPP
