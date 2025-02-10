#include "expr_nodes.hpp"
#include "stmt_nodes.hpp"
#include "definition_nodes.hpp"

namespace tlc::syntax {
  IntegerLiteral::IntegerLiteral(i64 const value)
    : NodeBase({}), m_value(value) {
  }

  FloatLiteral::FloatLiteral(f64 const value)
    : NodeBase({}), m_value(value) {
  }

  StringLiteral::StringLiteral(Str value, Vec<Node> placeholders)
    : NodeBase({std::move(placeholders)}), m_value(std::move(value)) {
  }

  BooleanLiteral::BooleanLiteral(bool value)
    : NodeBase({}), m_value(value) {
  }

  TernaryExpr::TernaryExpr(
    Node operand1, Node operand2, Node operand3,
    Str op1, Str op2
  )
    : NodeBase({operand1, operand2, operand3}),
      m_op1(std::move(op1)), m_op2(std::move(op2)) {
  }

  BinaryExpr::BinaryExpr(Node lhs, Node rhs, Str op)
    : NodeBase({lhs, rhs}), m_op(std::move(op)) {
  }

  UnaryExpr::UnaryExpr(Node operand, Str op)
    : NodeBase({operand}), m_op(std::move(op)) {
  }

  IdentifierBase::IdentifierBase(Vec<Str> path)
    : NodeBase({}), m_path(std::move(path)) {
  }

  TupleExpr::TupleExpr(Vec<Node> ids) : NodeBase(std::move(ids)) {
  }

  FunctionCallExpr::FunctionCallExpr(Node callee, Node args
  ) : NodeBase({callee, args}) {
  }

  // auto FunctionCallExpr::fromPipeExpr(
  //   ASTNode lhs, ASTNode rhs
  // ) -> Opt<FunctionCallExpr> {
  //   if (matchAstType<FunctionCallExpr>(rhs)) {
  //     Vec<ASTNode> args = astCast<TupleExpr>(
  //       astCast<FunctionCallExpr>(rhs).args()
  //     ).children();
  //     args.insert(args.begin(), lhs);
  //
  //     return FunctionCallExpr{
  //       astCast<FunctionCallExpr>(rhs).callee(), TupleExpr{std::move(args)}
  //     };
  //   }
  //
  //   if (matchAstType<VarId>(rhs)) {
  //     return FunctionCallExpr{rhs, {lhs}};
  //   }
  //
  //   return {};
  // }

  SubScriptingExpr::SubScriptingExpr(
    Node collection,
    Node subscript
  ): NodeBase({collection, subscript}) {
  }

  AccessExpr::AccessExpr(Node object, Node field)
    : NodeBase({object, field}) {
  }

  // TypeOfExpr::TypeOfExpr(ASTNode identifier)
  //   : ASTNodeBase({identifier}) {
  // }
  //
  // TypeExpr::TypeExpr(Vec<ASTNode> types)
  //   : ASTNodeBase(std::move(types)) {
  // }

  ArrayExpr::ArrayExpr(Vec<Node> elements) : NodeBase(std::move(elements)) {
  }
}
