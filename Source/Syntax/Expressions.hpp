#ifndef  TOYLANG_SYNTAX_EXPRESSIONS_HPP
#define  TOYLANG_SYNTAX_EXPRESSIONS_HPP

#include "Base.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class Identifier final : public Node {
  public:
    explicit Identifier(std::string name);

    auto name() const noexcept -> const std::string & {
      return m_name;
    }

  private:
    std::string m_name;
  };

  class BinaryExpr final : public Node {
  public:
    BinaryExpr(VNode lhs, VNode rhs, std::string op);

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

  class UnaryExpr final : public Node {
  public:
    UnaryExpr(VNode operand, std::string op);

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
}

#endif // TOYLANG_SYNTAX_EXPRESSIONS_HPP
