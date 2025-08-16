#include "parse_unit.fwd.hpp"

namespace tlc::parse {
    auto handleStmt(Context context) -> Opt<syntax::Node> {
        if (auto returnStmt = handleReturnStmt(
            Context::enter(EContext::ReturnStmt, context)); returnStmt) {
            return returnStmt;
        }
        if (auto deferStmt = handleDeferStmt(
            Context::enter(EContext::DeferStmt, context)); deferStmt) {
            return deferStmt;
        }
        if (auto blockStmt = handleBlockStmt(
            Context::enter(EContext::BlockStmt, context)); blockStmt) {
            return blockStmt;
        }
        if (auto matchStmt = handleMatchStmt(
            Context::enter(EContext::MatchStmt, context)); matchStmt) {
            return matchStmt;
        }
        if (auto loopStmt = handleLoopStmt(
            Context::enter(EContext::LoopStmt, context)); loopStmt) {
            return loopStmt;
        }
        if (auto declStmt = handleDeclStmt(
            Context::enter(EContext::DeclStmt, context)); declStmt) {
            return declStmt;
        }
        if (auto exprStmt = handleExprPrefixedStmt(
            Context::enter(EContext::ExprStmt, context)); exprStmt) {
            return exprStmt;
        }

        // todo:
        return {};
    }

    auto handleDeclStmt(Context context) -> Opt<syntax::Node> {
        auto decl = handleDecl(Context::enter(EContext::Decl, context))
            .value_or({});
        if (syntax::empty(decl) ||
            context.backtrackIf(!context.stream().match(lexeme::equal))) {
            return {};
        }

        auto initializer = handleExpr(Context::enter(EContext::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(
            initializer, EReason::MissingExpr
        );

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, EReason::MissingEnclosingSymbol
        );
        return syntax::stmt::Decl{
            std::move(decl), std::move(initializer), context.location()
        };
    }

    auto handleReturnStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::return_)) {
            return {};
        }

        auto returnExpr = handleExpr(Context::enter(EContext::Expr, context))
            .value_or({});
        context.emitIfLexemeNotPresent(
            lexeme::semicolon, EReason::MissingEnclosingSymbol
        );
        return syntax::stmt::Return{std::move(returnExpr), context.location()};
    }

    auto handleExprPrefixedStmt(Context context) -> Opt<syntax::Node> {
        auto prefixExpr = handleExpr(Context::enter(EContext::Expr, context))
            .value_or({});
        if (syntax::empty(prefixExpr)) {
            return {};
        }

        if (context.stream().match(syntax::isAssignmentOperator)) {
            context.to(EContext::AssignStmt);

            auto op = context.stream().current().lexeme();
            auto expr = handleExpr(Context::enter(EContext::Expr, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(expr, EReason::MissingExpr);

            context.emitIfLexemeNotPresent(
                lexeme::semicolon, EReason::MissingEnclosingSymbol
            );
            return syntax::stmt::Assign{
                std::move(prefixExpr), std::move(expr),
                std::move(op), context.location()
            };
        }

        if (context.stream().match(lexeme::equalGreater)) {
            context.to(EContext::CondStmt);

            auto thenStmt = handleStmt(Context::enter(EContext::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(thenStmt, EReason::MissingStmt);

            return syntax::stmt::Conditional{
                std::move(prefixExpr), std::move(thenStmt), context.location()
            };
        }

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, EReason::MissingEnclosingSymbol
        );
        return syntax::stmt::Expression{std::move(prefixExpr), context.location()};
    }

    auto handleLoopStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::for_)) {
            return {};
        }

        auto decl = handleDecl(Context::enter(EContext::Decl, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(decl, EReason::MissingDecl);
        context.emitIfLexemeNotPresent(lexeme::in, EReason::MissingKeyword);

        auto range = handleExpr(Context::enter(EContext::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(range, EReason::MissingExpr);

        auto body = handleBlockStmt(Context::enter(EContext::BlockStmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(range, EReason::MissingStmt);

        return syntax::stmt::Loop{
            std::move(decl), std::move(range),
            std::move(body), context.location()
        };
    }

    auto handleMatchStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::match)) {
            return {};
        }

        auto matchExpr = handleExpr(Context::enter(EContext::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(matchExpr, EReason::MissingExpr);

        if (context.emitIfLexemeNotPresent(
            lexeme::leftBrace, EReason::MissingBody)) {
            return syntax::stmt::Match{
                std::move(matchExpr), {}, {}, context.location()
            };
        }

        context.to(EContext::MatchCaseStmt);
        Vec<syntax::Node> cases;
        syntax::Node defaultStmt; // only the last default statement is considered
        while (context.stream().peek().lexeme() != lexeme::rightBrace) {
            if (context.stream().match(lexeme::anonymous)) {
                context.to(EContext::MatchCaseDefaultStmt);
                context.emitIfLexemeNotPresent(
                    lexeme::equalGreater, EReason::MissingSymbol
                );

                defaultStmt = handleStmt(Context::enter(EContext::Stmt, context))
                    .value_or(syntax::RequiredButMissing{});
                context.emitIfNodeMissing(defaultStmt, EReason::MissingStmt);
                continue;
            }

            auto caseCoords = context.stream().peek().location();
            auto value = handleExpr(Context::enter(EContext::Expr, context))
                .value_or({});

            syntax::Node cond;
            if (context.stream().match(lexeme::when)) {
                cond = handleExpr(Context::enter(EContext::Expr, context))
                    .value_or(syntax::RequiredButMissing{});
                context.emitIfNodeMissing(cond, EReason::MissingExpr);
            }

            context.emitIfLexemeNotPresent(
                lexeme::equalGreater, EReason::MissingSymbol
            );
            auto stmt = handleStmt(Context::enter(EContext::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(stmt, EReason::MissingStmt);

            cases.emplace_back(syntax::stmt::MatchCase{
                std::move(value), std::move(cond),
                std::move(stmt), std::move(caseCoords)
            });
        }

        context.to(EContext::MatchStmt);
        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, EReason::MissingEnclosingSymbol
        );
        return syntax::stmt::Match{
            std::move(matchExpr), std::move(cases),
            std::move(defaultStmt), context.location()
        };
    }

    auto handleBlockStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::leftBrace)) {
            return {};
        }

        Vec<syntax::Node> statements;
        while (context.stream().peek().lexeme() != lexeme::rightBrace) {
            auto stmt = handleStmt(Context::enter(EContext::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(stmt, EReason::MissingStmt);
            statements.push_back(std::move(stmt));
        }

        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, EReason::MissingEnclosingSymbol
        );
        return syntax::stmt::Block{std::move(statements), context.location()};
    }

    auto handleDeferStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::defer)) {
            return {};
        }

        auto stmt = handleStmt(Context::enter(EContext::Stmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(stmt, EReason::MissingStmt);
        return syntax::stmt::Defer{std::move(stmt), context.location()};
    }
}
