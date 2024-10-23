#ifndef  TOYLANG_SYNTAX_CONCRETE_HPP
#define  TOYLANG_SYNTAX_CONCRETE_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class TranslationUnit final : public Node {
  public:
    // todo
    explicit TranslationUnit(Vec<VNode> definitions);

    auto definition(const sz index) -> CRef<VNode> {
      return childAt(index);
    }

  private:
  };

  class Function final : public Node {
  public:
    Function(
      VNode fIdentifier, VNode fPrototype,
      VNode fBody,
      String storageSpecifier, bool pure
    );

    auto identifier() -> CRef<VNode> {
      return childAt(0);
    }

    auto prototype() -> CRef<VNode> {
      return childAt(1);
    }

    auto pure() const -> bool {
      return m_pure;
    }

    auto body() const -> CRef<VNode> {
      return childAt(2);
    }

    auto storage() const -> CRef<String> {
      return m_storage;
    }

  private:
    bool m_pure;
    String m_storage;
  };

  class Clazz final : public Node {
  public:
    Clazz(String visibility, Vec<VNode> parents, CRef<VNode> body);

    auto visibility() -> CRef<String> {
      return m_visibility;
    }

  private:
    String m_visibility;
  };

  class FunctionPrototype final : public Node {
  public:
    FunctionPrototype(
      CRef<VNode> returnTypeExpr,
      Vec<VNode> parameterDecls
    );

    auto returnTypeExpr() -> CRef<VNode> {
      return childAt(0);
    }
  };
}

#endif // TOYLANG_SYNTAX_CONCRETE_HPP
