#include "Expressions.hpp"

namespace tl::parser::syntax {
  BinaryExpr::BinaryExpr(VNode lhs, VNode rhs, std::string op)
    : Node({std::move(lhs), std::move(rhs)}), m_op(std::move(op)) {
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
