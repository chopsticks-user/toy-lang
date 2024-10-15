#include "Expressions.hpp"
#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  ParameterDeclFragment::ParameterDeclFragment(
    std::optional<VNode> identifierDeclFragment,
    std::string mut
  ) : Node({identifierDeclFragment.value()}), m_mutibility(std::move(mut)) {
  }

  IdentifierDeclFragment::IdentifierDeclFragment(
    std::optional<VNode> identifier,
    std::optional<VNode> typeExpr,
    std::optional<VNode> rhsExpr
  ) : Node({identifier.value(), typeExpr.value(), rhsExpr.value()}) {
  }

  Identifier::Identifier(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  TypeExpr::TypeExpr(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  BinaryExpr::BinaryExpr(std::optional<VNode> lhs, std::optional<VNode> rhs, std::string op)
    : Node({lhs.value(), rhs.value()}), m_op(std::move(op)) {
  }

  TernaryExpr::TernaryExpr(
    std::optional<VNode> operand1,
    std::optional<VNode> operand2,
    std::optional<VNode> operand3,
    std::string op1, std::string op2
  )
    : Node({operand1.value(), operand2.value(), operand3.value()}),
      m_op1(std::move(op1)), m_op2(std::move(op2)) {
  }

  UnaryExpr::UnaryExpr(std::optional<VNode> operand, std::string op)
    : Node({operand.value()}), m_op(std::move(op)) {
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

  FunctionCallExpr::FunctionCallExpr(
    std::optional<VNode> callee,
    std::vector<VNode> args
  ) : Node({
    [&]() {
      // todo: move args
      auto v = std::vector{callee.value()};
      v.insert(v.end(), args.begin(), args.end());
      return v;
    }()
  }) {
  }

  auto FunctionCallExpr::callee() const noexcept -> VNode {
    return childAt(0);
  }

  SubScriptingExpr::SubScriptingExpr(
    std::optional<VNode> collection,
    std::optional<VNode> subscript
  ): Node({collection.value(), subscript.value()}) {
  }

  auto SubScriptingExpr::collection() const noexcept -> VNode {
    return childAt(0);
  }

  auto SubScriptingExpr::subscript() const noexcept -> VNode {
    return childAt(1);
  }

  ModuleExpr::ModuleExpr(std::vector<VNode> fragments)
    : Node(std::move(fragments)) {
  }
}
