#ifndef  TOYLANG_SYNTAX_DEFINITIONS_HPP
#define  TOYLANG_SYNTAX_DEFINITIONS_HPP

#include "FrontEnd/Token.hpp"

#include "Base.hpp"
#include "core/Core.hpp"

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
    IdentifierDecl(bool isMutable, CRef<ASTNode> identifier, CRef<ASTNode> typeExpr);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto type() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto isMutable() const noexcept -> bool {
      return m_mutable;
    }

  private:
    bool m_mutable;
  };

  class TupleDecl final : public ASTNodeBase {
  public:
    explicit TupleDecl(Vec<ASTNode> idDecls);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto idDecl(sz index) const noexcept -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class GenericDecl final : public ASTNodeBase {
  public:
    explicit GenericDecl(Vec<ASTNode> typeIds);

    auto typeDecl(const sz index) -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class FunctionPrototype final : public ASTNodeBase {
  public:
    FunctionPrototype(
      FnType fType, ASTNode fIdentifier, ASTNode fParamDecls, ASTNode fReturnDecls
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

    auto type() const noexcept -> FnType {
      return m_type;
    }

    auto isAnonymous() const noexcept -> bool;

  private:
    FnType m_type;
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

    auto isLambda() const noexcept -> bool;

  private:
    Storage m_storage;
  };

  class ConceptDef final : public ASTNodeBase {
  public:
    ConceptDef(Storage storage, ASTNode identifier, Vec<ASTNode> requirements);

    auto identifier() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto requirement(const u64 index) const noexcept -> CRef<ASTNode> {
      return childAt(index + 1);
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

  class TokenNode final : public ASTNodeBase {
  public:
    explicit TokenNode(fe::Token token);

    auto token() const noexcept -> CRef<fe::Token> {
      return m_token;
    }

  private:
    fe::Token m_token;
  };
}

#endif // TOYLANG_SYNTAX_DEFINITIONS_HPP
