#ifndef  TLC_SYNTAX_STMT_NODES_HPP
#define  TLC_SYNTAX_STMT_NODES_HPP

#include "ast_base.hpp"
#include "core/core.hpp"

namespace tlc::syntax {
  class ForStmt final : public NodeBase {
  public:
    ForStmt(Node condition, Node body);

    auto condition() const noexcept -> Node const & {
      return childAt(0);
    }

    auto body() const noexcept -> Node const & {
      return childAt(1);
    }

    auto isForRange() const noexcept -> bool;
  };

  class ForRangeFragment final : public NodeBase {
  public:
    ForRangeFragment(Node iterator, Node iterable);

    auto iterator() const noexcept -> Node const & {
      return childAt(0);
    }

    auto iterable() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  class MatchStmt final : public NodeBase {
  public:
    MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases);

    auto nExprs() const noexcept -> sz;

    auto matchedExpr() const noexcept -> Node const & {
      return childAt(0);
    }

    auto defaultCase() const noexcept -> Node const & {
      return childAt(1);
    }

    auto caseAt(const u64 index) const noexcept -> Node const & {
      return childAt(index + 2);
    }

    auto nCases() const noexcept -> sz {
      return nChildren() - 1;
    }
  };

  class MatchStmtCase final : public NodeBase {
  public:
    MatchStmtCase(Node value, Node condition, Node body);

    auto value() const noexcept -> Node const & {
      return childAt(0);
    }

    auto condition() const noexcept -> Node const & {
      return childAt(1);
    }

    auto body() const noexcept -> Node const & {
      return childAt(2);
    }
  };

  class BlockStmt final : public NodeBase {
  public:
    explicit BlockStmt(Vec<Node> statements);

    auto stmt(const u64 index) -> Node const & {
      return childAt(index);
    }
  };

  class LetStmt final : public NodeBase {
  public:
    LetStmt(Node decl, Node init);

    auto decl() const noexcept -> Node const & {
      return childAt(0);
    }

    auto init() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  class ConditionalStmt final : public NodeBase {
  public:
    ConditionalStmt(Node condition, Node body);

    auto condition() const noexcept -> Node const & {
      return childAt(0);
    }

    auto body() const noexcept -> Node const & {
      return childAt(1);
    }
  };

  class ReturnStmt final : public NodeBase {
  public:
    explicit ReturnStmt(Node expr);

    auto expr() const noexcept -> Node const & {
      return childAt(0);
    }
  };

  class AssignStmt final : public NodeBase {
  public:
    AssignStmt(Node left, Node right, Str op);

    auto left() const noexcept -> Node const & {
      return childAt(0);
    }

    auto right() const noexcept -> Node const & {
      return childAt(1);
    }

    auto op() const noexcept -> StrV {
      return m_op;
    }

  private:
    Str m_op;
  };

  class ExprStmt final : public NodeBase {
  public:
    explicit ExprStmt(Node expr);

    auto expr() const noexcept -> Node const & {
      return childAt(0);
    }
  };
}

#endif // TLC_SYNTAX_STMT_NODES_HPP
