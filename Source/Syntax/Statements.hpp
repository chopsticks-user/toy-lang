#ifndef  TOYLANG_SYNTAX_STATEMENTS_HPP
#define  TOYLANG_SYNTAX_STATEMENTS_HPP

#include "Base.hpp"
#include "Expressions.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class BlockStatement final : public Node {
  public:
    explicit BlockStatement(std::vector<VNode> statements);

  private:
  };

  class IdentifierDeclStatement final : public Node {
  public:
    IdentifierDeclStatement(std::vector<VNode> fragments, std::string mut);

    auto mutibility() -> const std::string & {
      return m_mutibility;
    }

  private:
    std::string m_mutibility;
  };

  class ReturnStatement final : public Node {
  public:
    // don't know why I need to put std::optional<VNode> at the end,
    // took me half an hour but whatever
    explicit ReturnStatement(std::optional<VNode> expr, std::optional<VNode>);

    auto expression() -> const VNode & {
      return childAt(0);
    }

  private:
  };

  class AssignmentStatement final : public Node {
  public:
    AssignmentStatement(
      std::optional<VNode> left, std::optional<VNode> right, std::string op
    );

    auto left() -> const VNode & {
      return childAt(0);
    }

    auto right() -> const VNode & {
      return childAt(1);
    }

    auto op() -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class IfStatement final : public Node {
  public:
    IfStatement(
      std::optional<VNode> decl, std::optional<VNode> cond,
      std::optional<VNode> body, std::optional<VNode> elseBody
    );

    auto declStatement() -> const VNode & {
      return childAt(0);
    }

    auto condition() -> const VNode & {
      return childAt(1);
    }

    auto body() -> const VNode & {
      return childAt(2);
    }

    // may contain another if statement
    auto elseNode() -> const VNode & {
      return childAt(3);
    }

  private:
  };

  class ForStatement final : public Node {
  public:
    ForStatement(
      std::optional<VNode> decl, std::optional<VNode> cond,
      std::optional<VNode> postExpr, std::optional<VNode> loopBody
    );

    ForStatement(
      std::optional<VNode> it, std::optional<VNode> collection, std::optional<VNode> loopBody
    );

    auto isForRange() -> bool {
      // ranged-for loops only contains 2 elements, an iterator and a collection
      return nChildren() == 2;
    }

    auto iterator() -> const VNode & {
      return firstChild();
    }

    auto conditionOrCollection() -> const VNode & {
      return childAt(1);
    }

    auto postExpression() -> const VNode & {
      return childAt(2);
    }

    // may contain another if statement
    auto body() -> const VNode & {
      return lastChild();
    }

  private:
  };
}

#endif // TOYLANG_SYNTAX_STATEMENTS_HPP
