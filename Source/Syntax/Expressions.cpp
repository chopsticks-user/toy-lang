#include "Expressions.hpp"

namespace tl::syntax {
  Identifier::Identifier(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  BinaryExpr::BinaryExpr(VNode lhs, VNode rhs, std::string op)
    : Node({lhs, rhs}), m_op(std::move(op)) {
  }

  UnaryExpr::UnaryExpr(VNode operand, std::string op)
    : Node({operand}), m_op(std::move(op)) {
  }

  IntegerLiteral::IntegerLiteral(const std::string &value)
    : Node({}), m_value(std::stol(value)) {
  }

  FloatLiteral::FloatLiteral(const std::string &value)
    : Node({}), m_value(std::stod(value)) {
  }

  StringLiteral::StringLiteral(
    std::string value,
    std::vector<VNode> placeholders
  )
    : Node({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(const std::string &value)
    : Node({}), m_value(value == "true") {
  }
}
