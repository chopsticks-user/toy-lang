#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  BlockStatement::BlockStatement(Vec<VNode> statements)
    : Node(std::move(statements)) {
  }

  IdentifierDeclStatement::IdentifierDeclStatement(
    Vec<VNode> fragments, String mut
  ) : Node(std::move(fragments)), m_mutibility(std::move(mut)) {
  }

  ReturnStatement::ReturnStatement(const VNode &expr)
    : Node(isEmpty(expr) ? Vec<VNode>{} : std::vector{expr}) {
  }

  AssignmentStatement::AssignmentStatement(
    VNode left, VNode right, String op
  ): Node({left, right}), m_op(std::move(op)) {
  }

  IfStatement::IfStatement(
    VNode decl, VNode cond,
    VNode body, VNode elseBody
  ): Node({decl, cond, body, elseBody}) {
  }

  ForStatement::ForStatement(
    VNode decl, VNode cond,
    VNode postExpr, VNode loopBody
  ): Node({decl, cond, postExpr, loopBody}) {
  }

  ForStatement::ForStatement(
    VNode it, VNode collection, VNode loopBody
  ): Node({it, collection, loopBody}) {
  }
}
