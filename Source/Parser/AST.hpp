#ifndef TOYLANG_SYNTAX_AST_HPP
#define TOYLANG_SYNTAX_AST_HPP

#include "Core/Core.hpp"

namespace tl::parser::ast {
  class Program;
  class Function;
  class BinaryExpr;
  class UnaryExpr;
  class BlockScope;
  class Number;
  class Identifier;

  // using VNode = std::variant<Program, Function, BinaryExpr, UnaryExpr, BlockScope>;
  using VNode = std::variant<BinaryExpr, UnaryExpr, Number, Identifier>;

  template<sz NChildren>
  class Node {
  public:
    auto children() const noexcept -> const std::array<std::unique_ptr<VNode>, NChildren> & {
      return m_children;
    }

  protected:
    auto childAt(sz index) noexcept -> VNode & {
      return *m_children[index];
    }

    auto childAt(sz index) const noexcept -> VNode & {
      return *m_children[index];
    }

    explicit Node(std::array<std::unique_ptr<VNode>, NChildren> children) noexcept
      : m_children(std::move(children)) {
    }

  private:
    std::array<std::unique_ptr<VNode>, NChildren> m_children;
  };

  class BinaryExpr final : public Node<2> {
  public:
    BinaryExpr(std::unique_ptr<VNode> left, std::unique_ptr<VNode> right, std::string op)
      : Node({std::move(left), std::move(right)}), m_op(std::move(op)) {
    }

    auto left() const noexcept -> VNode & {
      return childAt(0);
    }

    auto right() const noexcept -> VNode & {
      return childAt(1);
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class UnaryExpr final : public Node<1> {
  public:
    UnaryExpr(std::unique_ptr<VNode> operand, std::string op)
      : Node({std::move(operand)}), m_op(std::move(op)) {
    }

    auto operand() const noexcept -> VNode & {
      return childAt(0);
    }

    auto op() const noexcept -> const std::string & {
      return m_op;
    }

  private:
    std::string m_op;
  };

  class Number final : public Node<0> {
  public:
    explicit Number(const std::string &value)
      : Node({}), m_value(std::stod(value)) {
    }

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
  };

  class Identifier final : public Node<0> {
  public:
    explicit Identifier(std::string name)
      : Node({}), m_name(std::move(name)) {
    }

    auto name() const noexcept -> const std::string & {
      return m_name;
    }

  private:
    std::string m_name;
  };
}


#endif // TOYLANG_SYNTAX_AST_HPP
