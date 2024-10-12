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
  class StringLiteral;

  using VNode = std::variant<BinaryExpr, Number, Identifier, UnaryExpr, StringLiteral>;

  // using VExpr = std::variant<BinaryExpr, UnaryExpr, Number, Identifier, StringLiteral>;

  class Node {
  public:
    auto children() const noexcept -> const std::vector<VNode> &;

    auto childAt(sz index) const -> const VNode &;

  protected:
    explicit Node(std::vector<VNode> children) noexcept;

    auto childAt(sz index) -> VNode &;

  private:
    std::vector<VNode> m_children;
  };

  class BinaryExpr final : public Node {
  public:
    BinaryExpr(VNode l, VNode r, std::string op);

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

  class Number final : public Node {
  public:
    explicit Number(const std::string &value);

    auto value() const noexcept -> double {
      return m_value;
    }

  private:
    f64 m_value;
    std::vector<VNode> m_children;
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
}


#endif // TOYLANG_SYNTAX_AST_HPP
