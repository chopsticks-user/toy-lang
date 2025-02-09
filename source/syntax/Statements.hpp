#ifndef  TOYLANG_SYNTAX_STATEMENTS_HPP
#define  TOYLANG_SYNTAX_STATEMENTS_HPP

#include "Base.hpp"
#include "core/Core.hpp"

namespace tl::syntax {
  class ForStmt final : public ASTNodeBase {
  public:
    ForStmt(ASTNode condition, ASTNode body);

    auto condition() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto body() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto isForRange() const noexcept -> bool;
  };

  class ForRangeFragment final : public ASTNodeBase {
  public:
    ForRangeFragment(ASTNode iterator, ASTNode iterable);

    auto iterator() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto iterable() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class MatchStmt final : public ASTNodeBase {
  public:
    MatchStmt(ASTNode matchedExpr, ASTNode defaultBody, Vec<ASTNode> cases);

    auto nExprs() const noexcept -> sz;

    auto matchedExpr() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto defaultCase() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto caseAt(const u64 index) const noexcept -> CRef<ASTNode> {
      return childAt(index + 2);
    }

    auto nCases() const noexcept -> sz {
      return nChildren() - 1;
    }
  };

  class MatchStmtCase final : public ASTNodeBase {
  public:
    MatchStmtCase(ASTNode value, ASTNode condition, ASTNode body);

    auto value() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto condition() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto body() const noexcept -> CRef<ASTNode> {
      return childAt(2);
    }
  };

  class BlockStmt final : public ASTNodeBase {
  public:
    explicit BlockStmt(Vec<ASTNode> statements);

    auto stmt(const u64 index) -> CRef<ASTNode> {
      return childAt(index);
    }
  };

  class LetStmt final : public ASTNodeBase {
  public:
    LetStmt(ASTNode decl, ASTNode init);

    auto decl() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto init() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class ConditionalStmt final : public ASTNodeBase {
  public:
    ConditionalStmt(ASTNode condition, ASTNode body);

    auto condition() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto body() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }
  };

  class ReturnStmt final : public ASTNodeBase {
  public:
    explicit ReturnStmt(ASTNode expr);

    auto expr() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }
  };

  class AssignStmt final : public ASTNodeBase {
  public:
    AssignStmt(ASTNode left, ASTNode right, String op);

    auto left() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }

    auto right() const noexcept -> CRef<ASTNode> {
      return childAt(1);
    }

    auto op() const noexcept -> const String & {
      return m_op;
    }

  private:
    String m_op;
  };

  class ExprStmt final : public ASTNodeBase {
  public:
    explicit ExprStmt(ASTNode expr);

    auto expr() const noexcept -> CRef<ASTNode> {
      return childAt(0);
    }
  };
}

#endif // TOYLANG_SYNTAX_STATEMENTS_HPP
