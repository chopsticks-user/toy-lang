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

  TupleExpr::TupleExpr(Vec<ASTNode> ids) : ASTNodeBase(std::move(ids)) {
  }

  FunctionCallExpr::FunctionCallExpr(ASTNode callee, ASTNode args
  ) : ASTNodeBase({callee, args}) {
  }

  auto FunctionCallExpr::fromPipeExpr(ASTNode lhs, ASTNode rhs) -> Opt<FunctionCallExpr> {
    if (matchAstType<FunctionCallExpr>(rhs)) {
      auto args = astCast<TupleExpr>(astCast<FunctionCallExpr>(rhs).args()).children();
      args.insert(args.begin(), lhs);

      return FunctionCallExpr{
        astCast<FunctionCallExpr>(rhs).callee(), TupleExpr{std::move(args)}
      };
    }

    if (matchAstType<Identifier>(rhs)) {
      return FunctionCallExpr{rhs, {lhs}};
    }

    return {};
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

  ArrayExpr::ArrayExpr(Vec<ASTNode> elements) : ASTNodeBase(std::move(elements)) {
  }
}
