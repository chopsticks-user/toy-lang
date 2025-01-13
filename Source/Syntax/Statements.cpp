#include "Statements.hpp"
#include "Expressions.hpp"
#include "Definitions.hpp"

namespace tl::syntax {
  ForStmt::ForStmt(ASTNode condition, ASTNode body)
    : ASTNodeBase({condition, body}) {
  }

  auto ForStmt::isForRange() const noexcept -> bool {
    return matchAstType<ForRangeFragment>(condition());
  }

  ForRangeFragment::ForRangeFragment(ASTNode iterator, ASTNode iterable)
    : ASTNodeBase({iterator, iterable}) {
  }

  MatchStmt::MatchStmt(ASTNode matchedExpr, ASTNode defaultBody, Vec<ASTNode> cases)
    : ASTNodeBase({
      [&]() {
        // todo: move args
        auto v = std::vector{matchedExpr, defaultBody};
        v.insert(v.end(), cases.begin(), cases.end());
        return v;
      }()
    }) {
  }

  auto MatchStmt::nExprs() const noexcept -> sz {
    const auto expr = matchedExpr();

    if (isEmptyAst(expr)) {
      return 0;
    }

    if (matchAstType<TupleExpr>(expr)) {
      return astCast<TupleExpr>(expr).size();
    }

    return 1;
  }

  MatchStmtCase::MatchStmtCase(ASTNode value, ASTNode condition, ASTNode body)
    : ASTNodeBase({value, condition, body}) {
  }

  BlockStmt::BlockStmt(Vec<ASTNode> statements)
    : ASTNodeBase(std::move(statements)) {
  }

  LetStmt::LetStmt(ASTNode decl, ASTNode init)
    : ASTNodeBase({decl, init}) {
  }

  ConditionalStmt::ConditionalStmt(ASTNode condition, ASTNode body)
    : ASTNodeBase({condition, body}) {
  }

  ReturnStmt::ReturnStmt(ASTNode expr)
    : ASTNodeBase({expr}) {
  }

  AssignStmt::AssignStmt(ASTNode left, ASTNode right, String op)
    : ASTNodeBase({left, right}), m_op(std::move(op)) {
  }

  ExprStmt::ExprStmt(ASTNode expr)
    : ASTNodeBase({expr}) {
  }
}
