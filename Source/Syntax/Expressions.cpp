#include "Expressions.hpp"

#include <utility>
#include "Statements.hpp"
#include "Definitions.hpp"

namespace tl::syntax {
  IntegerLiteral::IntegerLiteral(i64 value)
    : ASTNodeBase({}), m_value(value) {
  }

  FloatLiteral::FloatLiteral(f64 value)
    : ASTNodeBase({}), m_value(value) {
  }

  StringLiteral::StringLiteral(
    String value,
    Vec<ASTNode> placeholders
  )
    : ASTNodeBase({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(bool value)
    : ASTNodeBase({}), m_value(value) {
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

  BinaryExpr::BinaryExpr(ASTNode lhs, ASTNode rhs, String op)
    : ASTNodeBase({lhs, rhs}), m_op(std::move(op)) {
  }


  UnaryExpr::UnaryExpr(ASTNode operand, String op)
    : ASTNodeBase({operand}), m_op(std::move(op)) {
  }

  PostfixUnaryExpr::PostfixUnaryExpr(ASTNode operand, String op)
    : ASTNodeBase({operand}), m_op(std::move(op)) {
  }

  Identifier::Identifier(Vec<String> path)
    : ASTNodeBase({}), m_path(std::move(path)) {
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

  SubScriptingExpr::SubScriptingExpr(
    ASTNode collection,
    ASTNode subscript
  ): ASTNodeBase({collection, subscript}) {
  }

  AccessExpr::AccessExpr(ASTNode object, ASTNode field)
    : ASTNodeBase({object, field}) {
  }

  TypeOfExpr::TypeOfExpr(ASTNode identifier)
    : ASTNodeBase({identifier}) {
  }

  TypeExpr::TypeExpr(Vec<ASTNode> types)
    : ASTNodeBase(std::move(types)) {
  }
}
