#include "Expressions.hpp"
#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  ParameterDeclFragment::ParameterDeclFragment(
    VNode identifierDeclFragment,
    String mut
  ) : Node({identifierDeclFragment}),
      m_mutibility(mut.empty() ? "const" : std::move(mut)) {
  }

  IdentifierDeclFragment::IdentifierDeclFragment(
    CRef<VNode> identifier,
    CRef<VNode> typeExpr,
    CRef<VNode> rhsExpr
  ) : Node(
    isEmpty(rhsExpr)
      ? Vec<VNode>{identifier, typeExpr}
      : Vec<VNode>{identifier, typeExpr, rhsExpr}
  ) {
  }

  Identifier::Identifier(String name)
    : Node({}), m_name(std::move(name)) {
  }

  TypeExpr::TypeExpr(String name)
    : Node({}), m_name(std::move(name)) {
  }

  BinaryExpr::BinaryExpr(VNode lhs, VNode rhs, String op)
    : Node({lhs, rhs}), m_op(std::move(op)) {
  }

  TernaryExpr::TernaryExpr(
    VNode operand1,
    VNode operand2,
    VNode operand3,
    String op1, String op2
  )
    : Node({operand1, operand2, operand3}),
      m_op1(std::move(op1)), m_op2(std::move(op2)) {
  }

  UnaryExpr::UnaryExpr(VNode operand, String op)
    : Node({operand}), m_op(std::move(op)) {
  }

  PostfixUnaryExpr::PostfixUnaryExpr(VNode operand, String op)
    : Node({operand}), m_op(std::move(op)) {
  }

  IntegerLiteral::IntegerLiteral(CRef<String> value)
    : Node({}), m_value(std::stol(String(value))) {
  }

  FloatLiteral::FloatLiteral(CRef<String> value)
    : Node({}), m_value(std::stod(String(value))) {
  }

  StringLiteral::StringLiteral(
    String value,
    Vec<VNode> placeholders
  )
    : Node({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(CRef<String> value)
    : Node({}), m_value(value == "true") {
  }

  FunctionCallExpr::FunctionCallExpr(
    CRef<VNode> callee,
    Vec<VNode> args
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

  ModuleExpr::ModuleExpr(Vec<VNode> fragments)
    : Node(std::move(fragments)) {
  }

  ImportExpr::ImportExpr(Vec<VNode> fragments)
    : Node(std::move(fragments)) {
  }
}
