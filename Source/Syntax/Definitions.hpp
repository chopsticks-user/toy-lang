#ifndef  TOYLANG_SYNTAX_DEFINITIONS_HPP
#define  TOYLANG_SYNTAX_DEFINITIONS_HPP

#include "Base.hpp"
#include "Core/Core.hpp"

namespace tl::syntax {
  // declarations belong here to avoid circular dependencies
  // between Decl and Def

  class ModuleDecl final : public ASTNodeBase {
  public:
    explicit ModuleDecl(ASTNode nsIdentifier);

    auto identifier() const -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class ImportDecl final : public ASTNodeBase {
  public:
    explicit ImportDecl(ASTNode nsIdentifier);

    auto identifier() const -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class TypeDecl final : public ASTNodeBase {
  public:
    explicit TypeDecl(ASTNode identifier, ASTNode typeExpr);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    // todo: types
  };

  class IdentifierDecl final : public ASTNodeBase {
  public:
    IdentifierDecl(
      CRef<ASTNode> identifier, CRef<ASTNode> typeExpr, CRef<ASTNode> rhsExpr
    );

    auto identifier() -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto initExpr() -> CRef<ASTNode> {
      return childAt(2);
    }
  };

  class TupleDecl final : public ASTNodeBase {
  public:
    explicit TupleDecl(Vec<ASTNode> idDecls);
  };

  class FunctionDef final : public ASTNodeBase {
  public:
    FunctionDef(
      Storage fStorage, ASTNode fIdentifier,
      ASTNode fParamDecls, ASTNode fReturnDecls, ASTNode fBody
    );

    auto identifier() const -> CRef<ASTNode> {
      return childAt(0);
    }

    auto params() const -> CRef<ASTNode> {
      return childAt(1);
    }

    auto returns() const -> CRef<ASTNode> {
      return childAt(2);
    }

    auto body() const -> CRef<ASTNode> {
      return childAt(3);
    }

    auto storage() const -> Storage {
      return m_storage;
    }

  private:
    Storage m_storage;
  };

  class MethodDef final : public ASTNodeBase {
  public:
    MethodDef(
      Access mAccess, Virtuality mVirtuality, ASTNode mIdentifier,
      ASTNode mParamDecls, ASTNode mReturnDecls, ASTNode mBody
    );

    auto identifier() const -> CRef<ASTNode> {
      return childAt(0);
    }

    auto params() const -> CRef<ASTNode> {
      return childAt(1);
    }

    auto returns() -> CRef<ASTNode> {
      return childAt(2);
    }

    auto body() const -> CRef<ASTNode> {
      return childAt(3);
    }

    auto access() const -> Access {
      return m_access;
    }

    auto virtuality() const -> Virtuality {
      return m_virtuality;
    }

    // auto isConstructor() const -> bool { return false; }
    //
    // auto isOpOverload() const -> bool { return false; }

  private:
    Access m_access;
    Virtuality m_virtuality;
  };

  // class ClassDef final : public ASTNodeBase {
  // public:
  //   ClassDef(String visibility, Vec<ASTNode> parents, CRef<ASTNode> body);
  //
  //   auto visibility() -> CRef<String> {
  //     return m_visibility;
  //   }
  //
  // private:
  //   String m_visibility;
  // };

  class TranslationUnit final : public ASTNodeBase {
  public:
    // todo
    explicit TranslationUnit(Vec<ASTNode> definitions);

    auto definition(const sz index) -> CRef<ASTNode> {
      return childAt(index);
    }
  };
}

#endif // TOYLANG_SYNTAX_DEFINITIONS_HPP
