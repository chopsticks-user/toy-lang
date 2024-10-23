#ifndef  TOYLANG_SYNTAX_CONCRETE_HPP
#define  TOYLANG_SYNTAX_CONCRETE_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class TranslationUnit final : public ASTNodeBase {
  public:
    // todo
    explicit TranslationUnit(Vec<ASTNode> definitions);

    auto definition(const sz index) -> CRef<ASTNode> {
      return childAt(index);
    }

  private:
  };

  class Function final : public ASTNodeBase {
  public:
    Function(
      ASTNode fIdentifier, ASTNode fPrototype,
      ASTNode fBody,
      String storageSpecifier, bool pure
    );

    auto identifier() -> CRef<ASTNode> {
      return childAt(0);
    }

    auto prototype() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto pure() const -> bool {
      return m_pure;
    }

    auto body() const -> CRef<ASTNode> {
      return childAt(2);
    }

    auto storage() const -> CRef<String> {
      return m_storage;
    }

  private:
    bool m_pure;
    String m_storage;
  };

  class Clazz final : public ASTNodeBase {
  public:
    Clazz(String visibility, Vec<ASTNode> parents, CRef<ASTNode> body);

    auto visibility() -> CRef<String> {
      return m_visibility;
    }

  private:
    String m_visibility;
  };

  class FunctionPrototype final : public ASTNodeBase {
  public:
    FunctionPrototype(
      CRef<ASTNode> returnTypeExpr,
      Vec<ASTNode> parameterDecls
    );

    auto returnTypeExpr() -> CRef<ASTNode> {
      return childAt(0);
    }
  };
}

#endif // TOYLANG_SYNTAX_CONCRETE_HPP
