#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleStmt(Context context) -> Opt<syntax::Node> {
        if (auto returnStmt = handleReturnStmt(
            Context::enter(Context::ReturnStmt, context)); returnStmt) {
            return returnStmt;
        }
        if (auto deferStmt = handleDeferStmt(
            Context::enter(Context::DeferStmt, context)); deferStmt) {
            return deferStmt;
        }
        if (auto blockStmt = handleBlockStmt(
            Context::enter(Context::BlockStmt, context)); blockStmt) {
            return blockStmt;
        }
        if (auto matchStmt = handleMatchStmt(
            Context::enter(Context::MatchStmt, context)); matchStmt) {
            return matchStmt;
        }
        if (auto loopStmt = handleLoopStmt(
            Context::enter(Context::LoopStmt, context)); loopStmt) {
            return loopStmt;
        }
        if (auto declStmt = handleDeclStmt(
            Context::enter(Context::DeclStmt, context)); declStmt) {
            return declStmt;
        }
        if (auto exprStmt = handleExprPrefixedStmt(
            Context::enter(Context::ExprStmt, context)); exprStmt) {
            return exprStmt;
        }

        // todo:
        return {};
    }

    auto handleDeclStmt(Context context) -> Opt<syntax::Node> {
        auto decl = handleDecl(Context::enter(Context::Decl, context)).value_or({});
        if (syntax::isEmptyNode(decl) ||
            context.backtrackIf(!context.stream().match(lexeme::equal))) {
            return {};
        }

        auto initializer = handleExpr(Context::enter(Context::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(
            initializer, Reason::MissingExpr
        );

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::stmt::Decl{
            std::move(decl), std::move(initializer), context.location()
        };
    }

    auto handleReturnStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::return_)) {
            return {};
        }

        auto returnExpr = handleExpr(Context::enter(Context::Expr, context))
            .value_or({});
        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::stmt::Return{std::move(returnExpr), context.location()};
    }

    auto handleExprPrefixedStmt(Context context) -> Opt<syntax::Node> {
        auto prefixExpr = handleExpr(Context::enter(Context::Expr, context))
            .value_or({});
        if (syntax::isEmptyNode(prefixExpr)) {
            return {};
        }

        if (context.stream().match(syntax::isAssignmentOperator)) {
            context.to(Context::AssignStmt);

            auto op = context.stream().current().lexeme();
            auto expr = handleExpr(Context::enter(Context::Expr, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(expr, Reason::MissingExpr);

            context.emitIfLexemeNotPresent(
                lexeme::semicolon, Reason::MissingEnclosingSymbol
            );
            return syntax::stmt::Assign{
                std::move(prefixExpr), std::move(expr),
                std::move(op), context.location()
            };
        }

        if (context.stream().match(lexeme::equalGreater)) {
            context.to(Context::CondStmt);

            auto thenStmt = handleStmt(Context::enter(Context::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(thenStmt, Reason::MissingStmt);

            return syntax::stmt::Conditional{
                std::move(prefixExpr), std::move(thenStmt), context.location()
            };
        }

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::stmt::Expression{std::move(prefixExpr), context.location()};
    }

    auto handleLoopStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::for_)) {
            return {};
        }

        auto decl = handleDecl(Context::enter(Context::Decl, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(decl, Reason::MissingDecl);
        context.emitIfLexemeNotPresent(lexeme::in, Reason::MissingKeyword);

        auto range = handleExpr(Context::enter(Context::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(range, Reason::MissingExpr);

        auto body = handleBlockStmt(Context::enter(Context::BlockStmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(range, Reason::MissingStmt);

        return syntax::stmt::Loop{
            std::move(decl), std::move(range),
            std::move(body), context.location()
        };
    }

    auto handleMatchStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::match)) {
            return {};
        }

        auto matchExpr = handleExpr(Context::enter(Context::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(matchExpr, Reason::MissingExpr);

        if (context.emitIfLexemeNotPresent(
            lexeme::leftBrace, Reason::MissingBody)) {
            return syntax::stmt::Match{
                std::move(matchExpr), {}, {}, context.location()
            };
        }

        context.to(Context::MatchCaseStmt);
        Vec<syntax::Node> cases;
        syntax::Node defaultStmt; // only the last default statement is considered
        while (context.stream().peek().lexeme() != lexeme::rightBrace) {
            if (context.stream().match(lexeme::anonymous)) {
                context.to(Context::MatchCaseDefaultStmt);
                context.emitIfLexemeNotPresent(
                    lexeme::equalGreater, Reason::MissingSymbol
                );

                defaultStmt = handleStmt(Context::enter(Context::Stmt, context))
                    .value_or(syntax::RequiredButMissing{});
                context.emitIfNodeEmpty(defaultStmt, Reason::MissingStmt);
                continue;
            }

            auto caseCoords = context.stream().peek().location();
            auto value = handleExpr(Context::enter(Context::Expr, context))
                .value_or({});

            syntax::Node cond;
            if (context.stream().match(lexeme::when)) {
                cond = handleExpr(Context::enter(Context::Expr, context))
                    .value_or(syntax::RequiredButMissing{});
                context.emitIfNodeEmpty(cond, Reason::MissingExpr);
            }

            context.emitIfLexemeNotPresent(
                lexeme::equalGreater, Reason::MissingSymbol
            );
            auto stmt = handleStmt(Context::enter(Context::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(stmt, Reason::MissingStmt);

            cases.emplace_back(syntax::stmt::MatchCase{
                std::move(value), std::move(cond),
                std::move(stmt), std::move(caseCoords)
            });
        }

        context.to(Context::MatchStmt);
        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, Reason::MissingEnclosingSymbol
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
            auto stmt = handleStmt(Context::enter(Context::Stmt, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(stmt, Reason::MissingStmt);
            statements.push_back(std::move(stmt));
        }

        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, Reason::MissingEnclosingSymbol
        );
        return syntax::stmt::Block{std::move(statements), context.location()};
    }

    auto handleDeferStmt(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::defer)) {
            return {};
        }

        auto stmt = handleStmt(Context::enter(Context::Stmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(stmt, Reason::MissingStmt);
        return syntax::stmt::Defer{std::move(stmt), context.location()};
    }
}
