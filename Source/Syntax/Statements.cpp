#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  BlockStatement::BlockStatement(std::vector<VNode> statements)
    : Node(std::move(statements)) {
  }

  IdentifierDeclStatement::IdentifierDeclStatement(
    std::vector<VNode> fragments, std::string mut
  ) : Node(std::move(fragments)), m_mutibility(std::move(mut)) {
  }

  ReturnStatement::ReturnStatement(std::optional<VNode> expr, std::optional<VNode>)
    : Node({expr.value()}) {
  }

  AssignmentStatement::AssignmentStatement(
    std::optional<VNode> left, std::optional<VNode> right, std::string op
  ): Node({left.value(), right.value()}), m_op(std::move(op)) {
  }

  IfStatement::IfStatement(
    std::optional<VNode> decl, std::optional<VNode> cond,
    std::optional<VNode> body, std::optional<VNode> elseBody
  ): Node({decl.value(), cond.value(), body.value(), elseBody.value()}) {
  }

  ForStatement::ForStatement(
    std::optional<VNode> decl, std::optional<VNode> cond,
    std::optional<VNode> postExpr, std::optional<VNode> loopBody
  ): Node({decl.value(), cond.value(), postExpr.value(), loopBody.value()}) {
  }

  ForStatement::ForStatement(
    std::optional<VNode> it, std::optional<VNode> collection, std::optional<VNode> loopBody
  ): Node({it.value(), collection.value(), loopBody.value()}) {
  }
}
