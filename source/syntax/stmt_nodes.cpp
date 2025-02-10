#include "stmt_nodes.hpp"
#include "expr_nodes.hpp"
#include "definition_nodes.hpp"

namespace tlc::syntax {
  ForStmt::ForStmt(Node condition, Node body)
    : NodeBase({condition, body}) {
  }

  auto ForStmt::isForRange() const noexcept -> bool {
    return matchAstType<ForRangeFragment>(condition());
  }

  ForRangeFragment::ForRangeFragment(Node iterator, Node iterable)
    : NodeBase({iterator, iterable}) {
  }

  MatchStmt::MatchStmt(Node matchedExpr, Node defaultBody, Vec<Node> cases)
    : NodeBase({
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

  MatchStmtCase::MatchStmtCase(Node value, Node condition, Node body)
    : NodeBase({value, condition, body}) {
  }

  BlockStmt::BlockStmt(Vec<Node> statements)
    : NodeBase(std::move(statements)) {
  }

  LetStmt::LetStmt(Node decl, Node init)
    : NodeBase({decl, init}) {
  }

  ConditionalStmt::ConditionalStmt(Node condition, Node body)
    : NodeBase({condition, body}) {
  }

  ReturnStmt::ReturnStmt(Node expr)
    : NodeBase({expr}) {
  }

  AssignStmt::AssignStmt(Node left, Node right, Str op)
    : NodeBase({left, right}), m_op(std::move(op)) {
  }

  ExprStmt::ExprStmt(Node expr)
    : NodeBase({expr}) {
  }
}
