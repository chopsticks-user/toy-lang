#include "Expressions.hpp"
#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  ParameterDeclFragment::ParameterDeclFragment(
    VNode identifierDeclFragment,
    std::string mut
  ) : Node({identifierDeclFragment}),
      m_mutibility(mut.empty() ? "const" : std::move(mut)) {
  }

  IdentifierDeclFragment::IdentifierDeclFragment(
    VNode identifier,
    VNode typeExpr,
    VNode rhsExpr
  ) : Node({identifier, typeExpr, rhsExpr}) {
  }

  Identifier::Identifier(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  TypeExpr::TypeExpr(std::string name)
    : Node({}), m_name(std::move(name)) {
  }

  BinaryExpr::BinaryExpr(VNode lhs, VNode rhs, std::string op)
    : Node({lhs, rhs}), m_op(std::move(op)) {
  }

  TernaryExpr::TernaryExpr(
    VNode operand1,
    VNode operand2,
    VNode operand3,
    std::string op1, std::string op2
  )
    : Node({operand1, operand2, operand3}),
      m_op1(std::move(op1)), m_op2(std::move(op2)) {
  }

  UnaryExpr::UnaryExpr(VNode operand, std::string op)
    : Node({operand}), m_op(std::move(op)) {
  }

  PostfixUnaryExpr::PostfixUnaryExpr(VNode operand, std::string op)
    : Node({operand}), m_op(std::move(op)) {
  }

  IntegerLiteral::IntegerLiteral(const String &value)
    : Node({}), m_value(std::stol(std::string(value))) {
  }

  FloatLiteral::FloatLiteral(const String &value)
    : Node({}), m_value(std::stod(std::string(value))) {
  }

  StringLiteral::StringLiteral(
    std::string value,
    std::vector<VNode> placeholders
  )
    : Node({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(const String &value)
    : Node({}), m_value(value == "true") {
  }

  FunctionCallExpr::FunctionCallExpr(
    const VNode &callee,
    std::vector<VNode> args
  ) : Node({
    [&]() {
      // todo: move args
      auto v = std::vector{callee};
      v.insert(v.end(), args.begin(), args.end());
      return v;
    }()
  }) {
  }

  auto FunctionCallExpr::callee() const noexcept -> VNode {
    return childAt(0);
  }

  SubScriptingExpr::SubScriptingExpr(
    VNode collection,
    VNode subscript
  ): Node({collection, subscript}) {
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

  ImportExpr::ImportExpr(std::vector<VNode> fragments)
    : Node(std::move(fragments)) {
  }
}
