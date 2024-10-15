#ifndef  TOYLANG_SYNTAX_CONCRETE_HPP
#define  TOYLANG_SYNTAX_CONCRETE_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class TranslationUnit final : public Node {
  public:
    // todo
    explicit TranslationUnit(std::vector<VNode> definitions);

    auto definition(sz index) -> const VNode & {
      return childAt(index);
    }

  private:
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

  class Clazz final : public Node {
  public:
    Clazz(std::string visibility, std::vector<VNode> parents, std::optional<VNode> body);

    auto visibility() -> const std::string & {
      return m_visibility;
    }

  private:
    std::string m_visibility;
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
