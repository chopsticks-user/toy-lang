#ifndef  TOYLANG_SYNTAX_EXPRESSIONS_HPP
#define  TOYLANG_SYNTAX_EXPRESSIONS_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class ParameterDeclFragment final : public Node {
  public:
    ParameterDeclFragment(
      std::optional<VNode> identifierDeclFragment,
      std::string mut
    );

    auto identifier() -> const VNode & {
      return childAt(0);
    }

    auto typeExpr() -> const VNode & {
      return childAt(1);
    }

    auto mutibility() -> const std::string & {
      return m_mutibility;
    }

  private:
    std::string m_mutibility;
  };

  class IdentifierDeclFragment final : public Node {
  public:
    IdentifierDeclFragment(
      std::optional<VNode> identifier,
      std::optional<VNode> typeExpr,
      std::optional<VNode> rhsExpr
    );

    auto identifier() -> const VNode & {
      return childAt(0);
    }

    auto typeExpr() -> const VNode & {
      return childAt(1);
    }

    auto rhsExpr() -> const VNode & {
      return childAt(2);
    }

  private:
  };

  class Identifier final : public Node {
  public:
    explicit Identifier(std::string name);

    auto name() const noexcept -> const std::string & {
      return m_name;
    }

  private:
    std::string m_name;
  };

  class TypeExpr final : public Node {
  public:
    explicit TypeExpr(std::string name);

    auto name() const noexcept -> const std::string & {
      return m_name;
    }

  private:
    std::string m_name;
  };

  class BinaryExpr final : public Node {
  public:
    BinaryExpr(std::optional<VNode> lhs, std::optional<VNode> rhs, std::string op);

    auto left() const noexcept -> const VNode & {
      return childAt(0);
    }

    auto right() const noexcept -> const VNode & {
      return childAt(1);
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class TernaryExpr final : public Node {
  public:
    TernaryExpr(
      std::optional<VNode> operand1,
      std::optional<VNode> operand2,
      std::optional<VNode> operand3,
      std::string op1, std::string op2
    );

    auto first() const noexcept -> const VNode & {
      return childAt(0);
    }

    auto second() const noexcept -> const VNode & {
      return childAt(1);
    }

    auto third() const noexcept -> const VNode & {
      return childAt(2);
    }

    auto firstOp() const noexcept -> const std::string & {
      return m_op1;
    }

    auto secondOp() const noexcept -> const std::string & {
      return m_op2;
    }

  private:
    std::string m_op1;
    std::string m_op2;
  };

  class UnaryExpr final : public Node {
  public:
    UnaryExpr(std::optional<VNode> operand, std::string op);

    auto operand() const noexcept -> const VNode & {
      return childAt(0);
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  // Literals belong here to avoid circular dependencies
  // between Expression and Literals

  class IntegerLiteral final : public Node {
  public:
    explicit IntegerLiteral(const std::string &value);

    auto value() const noexcept -> i64 {
      return m_value;
    }

  private:
    i64 m_value;
  };

  class FloatLiteral final : public Node {
  public:
    explicit FloatLiteral(const std::string &value);

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  class StringLiteral final : public Node {
  public:
    explicit StringLiteral(
      std::string value,
      std::vector<VNode> placeholders = {}
    );

    auto value() const noexcept -> const std::string & {
      return m_value;
    }

    auto placeholder(sz index) const noexcept -> const VNode & {
      return childAt(index);
    }

  private:
    std::string m_value;
  };

  class BooleanLiteral final : public Node {
  public:
    explicit BooleanLiteral(const std::string &value);

    auto value() const noexcept -> bool {
      return m_value;
    }

  private:
    bool m_value;
  };

  class FunctionCallExpr final : public Node {
  public:
    FunctionCallExpr(
      std::optional<VNode> callee,
      std::vector<VNode> args
    );

    auto callee() const noexcept -> VNode;
  };

  class SubScriptingExpr final : public Node {
  public:
    SubScriptingExpr(
      std::optional<VNode> collection,
      std::optional<VNode> subscript
    );

    auto collection() const noexcept -> VNode;

    auto subscript() const noexcept -> VNode;
  };
}

#endif // TOYLANG_SYNTAX_EXPRESSIONS_HPP
