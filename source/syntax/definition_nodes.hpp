#ifndef  TLC_SYNTAX_DEFINITION_NODES_HPP
#define  TLC_SYNTAX_DEFINITION_NODES_HPP

#include "ast_base.hpp"
#include "token/token.hpp"
#include "core/core.hpp"

namespace tlc::syntax {
  // declarations belong here to avoid circular dependencies
  // between Decl and Def

  class ModuleDecl final : public NodeBase {
  public:
    explicit ModuleDecl(Node nsIdentifier);

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }
  };

  class ImportDecl final : public NodeBase {
  public:
    explicit ImportDecl(Node nsIdentifier);

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }
  };

  class TypeDecl final : public NodeBase {
  public:
    explicit TypeDecl(Storage storage, Node identifier, Node typeExpr);

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }

    auto typeExpr() const noexcept -> Node const & {
      return childAt(1);
    }

    auto storage() const noexcept -> Storage {
      return m_storage;
    }

  private:
    Storage m_storage;
  };

  class IdentifierDecl final : public NodeBase {
  public:
    IdentifierDecl(bool isMutable, Node const &identifier, Node const &typeExpr);

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }

    auto type() const noexcept -> Node const & {
      return childAt(1);
    }

    auto isMutable() const noexcept -> bool {
      return m_mutable;
    }

  private:
    bool m_mutable;
  };

  class TupleDecl final : public NodeBase {
  public:
    explicit TupleDecl(Vec<Node> idDecls);

    auto size() const noexcept -> sz {
      return nChildren();
    }

    auto idDecl(sz index) const noexcept -> Node const & {
      return childAt(index);
    }
  };

  class GenericDecl final : public NodeBase {
  public:
    explicit GenericDecl(Vec<Node> typeIds);

    auto typeDecl(const sz index) -> Node const & {
      return childAt(index);
    }
  };

  class FunctionPrototype final : public NodeBase {
  public:
    FunctionPrototype(
      FnType fType, Node fIdentifier, Node fParamDecls, Node fReturnDecls
    );

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }

    auto params() const noexcept -> Node const & {
      return childAt(1);
    }

    auto returns() const noexcept -> Node const & {
      return childAt(2);
    }

    auto type() const noexcept -> FnType {
      return m_type;
    }

    auto isAnonymous() const noexcept -> bool;

  private:
    FnType m_type;
  };

  class FunctionDef final : public NodeBase {
  public:
    FunctionDef(Storage fStorage, Node fPrototype, Node fBody);

    auto prototype() const noexcept -> Node const & {
      return childAt(0);
    }

    auto body() const noexcept -> Node const & {
      return childAt(1);
    }

    auto storage() const noexcept -> Storage {
      return m_storage;
    }

    auto isLambda() const noexcept -> bool;

  private:
    Storage m_storage;
  };

  class ConceptDef final : public NodeBase {
  public:
    ConceptDef(Storage storage, Node identifier, Vec<Node> requirements);

    auto identifier() const noexcept -> Node const & {
      return childAt(0);
    }

    auto requirement(const u64 index) const noexcept -> Node const & {
      return childAt(index + 1);
    }

    auto storage() const noexcept -> Storage {
      return m_storage;
    }

  private:
    Storage m_storage;
  };

  class TranslationUnit final : public NodeBase {
  public:
    // todo
    explicit TranslationUnit(Vec<Node> definitions);

    auto definition(const sz index) const noexcept -> Node const & {
      return childAt(index);
    }
  };

  class TokenNode final : public NodeBase {
  public:
    explicit TokenNode(token::Token token);

    auto token() const noexcept -> token::Token const & {
      return m_token;
    }

  private:
    token::Token m_token;
  };
}

#endif // TLC_SYNTAX_DEFINITION_NODES_HPP
