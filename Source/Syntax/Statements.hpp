#ifndef  TOYLANG_SYNTAX_STATEMENTS_HPP
#define  TOYLANG_SYNTAX_STATEMENTS_HPP

#include "Base.hpp"
#include "Expressions.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class BlockStatement final : public ASTNodeBase {
  public:
    explicit BlockStatement(Vec<ASTNode> statements);

  private:
  };

  class IdentifierDeclStatement final : public ASTNodeBase {
  public:
    IdentifierDeclStatement(Vec<ASTNode> fragments, String mut);

    auto mutibility() -> const String & {
      return m_mutibility;
    }

  private:
    String m_mutibility;
  };

  class ReturnStatement final : public ASTNodeBase {
  public:
    explicit ReturnStatement(CRef<ASTNode> expr);

    auto expression() -> CRef<ASTNode> {
      return childAt(0);
    }

  private:
  };

  class AssignmentStatement final : public ASTNodeBase {
  public:
    AssignmentStatement(
      ASTNode left, ASTNode right, String op
    );

    auto left() -> CRef<ASTNode> {
      return childAt(0);
    }

    auto right() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto op() -> const String & {
      return m_op;
    }

  private:
    String m_op;
  };

  class IfStatement final : public ASTNodeBase {
  public:
    IfStatement(
      ASTNode decl, ASTNode cond,
      ASTNode body, ASTNode elseBody
    );

    auto declStatement() -> CRef<ASTNode> {
      return childAt(0);
    }

    auto condition() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto body() -> CRef<ASTNode> {
      return childAt(2);
    }

    // may contain another if statement
    auto elseNode() -> CRef<ASTNode> {
      return childAt(3);
    }

  private:
  };

  class ForStatement final : public ASTNodeBase {
  public:
    ForStatement(
      ASTNode decl, ASTNode cond,
      ASTNode postExpr, ASTNode loopBody
    );

    ForStatement(
      ASTNode it, ASTNode collection, ASTNode loopBody
    );

    auto isForRange() const -> bool {
      // ranged-for loops only contains 2 elements, an iterator and a collection
      return nChildren() == 2;
    }

    auto iterator() -> CRef<ASTNode> {
      return firstChild();
    }

    auto conditionOrCollection() -> CRef<ASTNode> {
      return childAt(1);
    }

    auto postExpression() -> CRef<ASTNode> {
      return childAt(2);
    }

    // may contain another if statement
    auto body() -> CRef<ASTNode> {
      return lastChild();
    }

  private:
  };
}

#endif // TOYLANG_SYNTAX_STATEMENTS_HPP
