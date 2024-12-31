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

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class ImportDecl final : public ASTNodeBase {
  public:
    explicit ImportDecl(ASTNode nsIdentifier);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class TypeDecl final : public ASTNodeBase {
  public:
    explicit TypeDecl(Storage storage, ASTNode identifier, ASTNode typeExpr);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto storage() const noexcept -> Storage {
      return m_storage;
    }

  private:
    Storage m_storage;
  };

  class IdentifierDecl final : public ASTNodeBase {
  public:
    IdentifierDecl(
      CRef<ASTNode> identifier, CRef<ASTNode> typeExpr, CRef<ASTNode> rhsExpr
    );

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto typeExpr() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto initExpr() const noexcept -> CRef<ASTNode> {
      return childAt(2);
    }
  };

  class TupleDecl final : public ASTNodeBase {
  public:
    explicit TupleDecl(Vec<ASTNode> idDecls);
  };

  class FunctionPrototype final : public ASTNodeBase {
  public:
    FunctionPrototype(
      bool fStatic, FnType fType, ASTNode fIdentifier,
      ASTNode fParamDecls, ASTNode fReturnDecls
    );

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto params() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto returns() const noexcept -> CRef<ASTNode> {
      return childAt(2);
    }

    auto isStatic() const noexcept -> bool {
      return m_static;
    }

    auto type() const noexcept -> FnType {
      return m_type;
    }

  private:
    FnType m_type;
    bool m_static;
  };

  class FunctionDef final : public ASTNodeBase {
  public:
    FunctionDef(Storage fStorage, ASTNode fPrototype, ASTNode fBody);

    auto prototype() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto body() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto storage() const noexcept -> Storage {
      return m_storage;
    }

  private:
    Storage m_storage;
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

    auto definition(const sz index) const noexcept -> CRef<ASTNode> {
      return childAt(index);
    }
  };
}

#endif // TOYLANG_SYNTAX_DEFINITIONS_HPP
