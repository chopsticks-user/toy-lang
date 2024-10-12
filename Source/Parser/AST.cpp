#include "AST.hpp"


namespace tl::parser::ast {
  Node::Node(std::vector<VNode> children) noexcept
    : m_children(std::move(children)) {
  }

  auto Node::children() const noexcept -> const std::vector<VNode> & {
    return m_children;
  }

  auto Node::childAt(sz index) const -> const VNode & {
    return m_children.at(index);
  }

  auto Node::childAt(sz index) -> VNode & {
    return m_children.at(index);
  }

  BinaryExpr::BinaryExpr(VNode l, VNode r, std::string op)
    : Node({std::move(l), std::move(r)}), m_op(std::move(op)) {
  }

  UnaryExpr::UnaryExpr(VNode operand, std::string op)
    : Node({std::move(operand)}), m_op(std::move(op)) {
  }

  Number::Number(const std::string &value)
    : Node({}), m_value(std::stod(value)) {
  }

  Identifier::Identifier(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  StringLiteral::StringLiteral(
    std::string value,
    std::vector<VNode> placeholders
  )
    : Node({std::move(placeholders)}), m_value(std::move(value)) {
  }
}
