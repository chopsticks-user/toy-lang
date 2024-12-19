// #include "Statements.hpp"
// #include "Definitions.hpp"
//
// namespace tl::syntax {
//   BlockStatement::BlockStatement(Vec<ASTNode> statements)
//     : ASTNodeBase(std::move(statements)) {
//   }
//
//   IdentifierDeclStatement::IdentifierDeclStatement(
//     Vec<ASTNode> fragments, String mut
//   ) : ASTNodeBase(std::move(fragments)), m_mutibility(std::move(mut)) {
//   }
//
//   ReturnStatement::ReturnStatement(const ASTNode &expr)
//     : ASTNodeBase(isEmpty(expr) ? Vec<ASTNode>{} : std::vector{expr}) {
//   }
//
//   AssignmentStatement::AssignmentStatement(
//     ASTNode left, ASTNode right, String op
//   ): ASTNodeBase({left, right}), m_op(std::move(op)) {
//   }
//
//   IfStatement::IfStatement(
//     ASTNode decl, ASTNode cond,
//     ASTNode body, ASTNode elseBody
//   ): ASTNodeBase({decl, cond, body, elseBody}) {
//   }
//
//   ForStatement::ForStatement(
//     ASTNode decl, ASTNode cond,
//     ASTNode postExpr, ASTNode loopBody
//   ): ASTNodeBase({decl, cond, postExpr, loopBody}) {
//   }
//
//   ForStatement::ForStatement(
//     ASTNode it, ASTNode collection, ASTNode loopBody
//   ): ASTNodeBase({it, collection, loopBody}) {
//   }
// }
