#include "Expressions.hpp"
#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  ParameterDeclFragment::ParameterDeclFragment(
    ASTNode identifierDeclFragment,
    String mut
  ) : ASTNodeBase({identifierDeclFragment}),
      m_mutibility(mut.empty() ? "const" : std::move(mut)) {
  }

  IdentifierDeclFragment::IdentifierDeclFragment(
    CRef<ASTNode> identifier,
    CRef<ASTNode> typeExpr,
    CRef<ASTNode> rhsExpr
  ) : ASTNodeBase(
    isEmpty(rhsExpr)
      ? Vec<ASTNode>{identifier, typeExpr}
      : Vec<ASTNode>{identifier, typeExpr, rhsExpr}
  ) {
  }

  Identifier::Identifier(String name)
    : ASTNodeBase({}), m_name(std::move(name)) {
  }

  TypeExpr::TypeExpr(String name)
    : ASTNodeBase({}), m_name(std::move(name)) {
  }

  BinaryExpr::BinaryExpr(ASTNode lhs, ASTNode rhs, String op)
    : ASTNodeBase({lhs, rhs}), m_op(std::move(op)) {
  }

  TernaryExpr::TernaryExpr(
    ASTNode operand1,
    ASTNode operand2,
    ASTNode operand3,
    String op1, String op2
  )
    : ASTNodeBase({operand1, operand2, operand3}),
      m_op1(std::move(op1)), m_op2(std::move(op2)) {
  }

  UnaryExpr::UnaryExpr(ASTNode operand, String op)
    : ASTNodeBase({operand}), m_op(std::move(op)) {
  }

  PostfixUnaryExpr::PostfixUnaryExpr(ASTNode operand, String op)
    : ASTNodeBase({operand}), m_op(std::move(op)) {
  }

  IntegerLiteral::IntegerLiteral(CRef<String> value)
    : ASTNodeBase({}), m_value(std::stol(String(value))) {
  }

  FloatLiteral::FloatLiteral(CRef<String> value)
    : ASTNodeBase({}), m_value(std::stod(String(value))) {
  }

  StringLiteral::StringLiteral(
    String value,
    Vec<ASTNode> placeholders
  )
    : ASTNodeBase({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(CRef<String> value)
    : ASTNodeBase({}), m_value(value == "true") {
  }

  FunctionCallExpr::FunctionCallExpr(
    CRef<ASTNode> callee,
    Vec<ASTNode> args
  ) : ASTNodeBase({
    [&]() {
      // todo: move args
      auto v = std::vector{callee};
      v.insert(v.end(), args.begin(), args.end());
      return v;
    }()
  }) {
  }

  auto FunctionCallExpr::callee() const noexcept -> ASTNode {
    return childAt(0);
  }

  SubScriptingExpr::SubScriptingExpr(
    ASTNode collection,
    ASTNode subscript
  ): ASTNodeBase({collection, subscript}) {
  }

  auto SubScriptingExpr::collection() const noexcept -> ASTNode {
    return childAt(0);
  }

  auto SubScriptingExpr::subscript() const noexcept -> ASTNode {
    return childAt(1);
  }

  ModuleExpr::ModuleExpr(Vec<ASTNode> fragments)
    : ASTNodeBase(std::move(fragments)) {
  }

  ImportExpr::ImportExpr(Vec<ASTNode> fragments)
    : ASTNodeBase(std::move(fragments)) {
  }
}
