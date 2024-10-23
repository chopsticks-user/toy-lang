#ifndef  TOYLANG_SYNTAX_STATEMENTS_HPP
#define  TOYLANG_SYNTAX_STATEMENTS_HPP

#include "Base.hpp"
#include "Expressions.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class BlockStatement final : public Node {
  public:
    explicit BlockStatement(Vec<VNode> statements);

  private:
  };

  class IdentifierDeclStatement final : public Node {
  public:
    IdentifierDeclStatement(Vec<VNode> fragments, String mut);

    auto mutibility() -> const String & {
      return m_mutibility;
    }

  private:
    String m_mutibility;
  };

  class ReturnStatement final : public Node {
  public:
    explicit ReturnStatement(CRef<VNode> expr);

    auto expression() -> CRef<VNode> {
      return childAt(0);
    }

  private:
  };

  class AssignmentStatement final : public Node {
  public:
    AssignmentStatement(
      VNode left, VNode right, String op
    );

    auto left() -> CRef<VNode> {
      return childAt(0);
    }

    auto right() -> CRef<VNode> {
      return childAt(1);
    }

    auto op() -> const String & {
      return m_op;
    }

  private:
    String m_op;
  };

  class IfStatement final : public Node {
  public:
    IfStatement(
      VNode decl, VNode cond,
      VNode body, VNode elseBody
    );

    auto declStatement() -> CRef<VNode> {
      return childAt(0);
    }

    auto condition() -> CRef<VNode> {
      return childAt(1);
    }

    auto body() -> CRef<VNode> {
      return childAt(2);
    }

    // may contain another if statement
    auto elseNode() -> CRef<VNode> {
      return childAt(3);
    }

  private:
  };

  class ForStatement final : public Node {
  public:
    ForStatement(
      VNode decl, VNode cond,
      VNode postExpr, VNode loopBody
    );

    ForStatement(
      VNode it, VNode collection, VNode loopBody
    );

    auto isForRange() const -> bool {
      // ranged-for loops only contains 2 elements, an iterator and a collection
      return nChildren() == 2;
    }

    auto iterator() -> CRef<VNode> {
      return firstChild();
    }

    auto conditionOrCollection() -> CRef<VNode> {
      return childAt(1);
    }

    auto postExpression() -> CRef<VNode> {
      return childAt(2);
    }

    // may contain another if statement
    auto body() -> CRef<VNode> {
      return lastChild();
    }

  private:
  };
}

#endif // TOYLANG_SYNTAX_STATEMENTS_HPP
