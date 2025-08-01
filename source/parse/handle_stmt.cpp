#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        if (auto returnStmt = handleReturnStmt(); returnStmt) {
            return returnStmt;
        }
        if (auto letStmt = handleLetStmt(); letStmt) {
            return letStmt;
        }
        if (auto deferStmt = handleDeferStmt(); deferStmt) {
            return deferStmt;
        }
        if (auto prefaceStmt = handlePrefaceStmt(); prefaceStmt) {
            return prefaceStmt;
        }
        if (auto yieldStmt = handleYieldStmt(); yieldStmt) {
            return yieldStmt;
        }
        if (auto blockStmt = handleBlockStmt(); blockStmt) {
            return blockStmt;
        }
        if (auto matchStmt = handleMatchStmt(); matchStmt) {
            return matchStmt;
        }
        if (auto loopStmt = handleLoopStmt(); loopStmt) {
            return loopStmt;
        }
        if (auto exprStmt = handleExprPrefixedStmt(); exprStmt) {
            return exprStmt;
        }
        // todo:
        return {};
    }

    auto Parse::handleLetStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::let)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();

                auto decl = *parseDecl().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::Stmt,
                            .reason = EParseErrorReason::MissingDecl,
                        });
                        return {};
                    }
                );

                auto initializer =
                    *match(lexeme::equal)(m_stream, m_tracker)
                     .and_then([this](auto) -> ParseResult {
                         return *parseExpr().or_else([this](auto&& error) -> ParseResult {
                             collect(error);
                             return {};
                         });
                     }).or_else([this](auto) -> ParseResult { return {}; });

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Let{
                    std::move(decl), std::move(initializer), location
                };
            }
        );
    }

    auto Parse::handleReturnStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::return_)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();

                auto returnStmt = syntax::stmt::Return{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        collect(error);
                        return {};
                    }),
                    location,
                };

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return returnStmt;
            }
        );
    }

    auto Parse::handleExprPrefixedStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();

        return handleExpr().and_then([&](auto&& expr)
            -> ParseResult {
                if (m_stream.match(syntax::isAssignmentOperator)) {
                    auto const op = m_stream.current().lexeme();
                    expr = syntax::stmt::Assign{
                        expr, *handleExpr().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::AssignStmt,
                                    .reason = EParseErrorReason::MissingExpr,
                                });
                                return {};
                            }
                        ),
                        op, *location
                    };
                }
                else if (m_stream.match(lexeme::equalGreater)) {
                    expr = syntax::stmt::Conditional{
                        expr, *handleStmt().or_else(
                            [&](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::CondStmt,
                                    .reason = EParseErrorReason::MissingStmt,
                                });
                                return {};
                            }
                        ),
                        *location
                    };
                }
                else {
                    expr = syntax::stmt::Expression{expr, *location};
                }

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }
                return expr;
            });
    }

    auto Parse::handleLoopStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::for_)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                auto decl = *handleStmtLevelDecl().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::LoopStmt,
                            .reason = EParseErrorReason::MissingDecl,
                        });
                        return {};
                    }
                );
                if (!m_stream.match(lexeme::in)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::LoopStmt,
                        .reason = EParseErrorReason::MissingKeyword,
                    });
                }
                auto range = *handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::LoopStmt,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return {};
                    }
                );
                auto body = *handleBlockStmt().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::LoopStmt,
                            .reason = EParseErrorReason::MissingStmt,
                        });
                        return {};
                    }
                );
                return syntax::stmt::Loop{
                    std::move(decl), std::move(range),
                    std::move(body), location
                };
            }
        );
    }

    auto Parse::handleMatchStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::match)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                auto expr = *handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::MatchStmt,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return {};
                    }
                );

                Vec<syntax::Node> cases;
                syntax::Node defaultStmt; // only the last default statement is considered
                if (!m_stream.match(lexeme::leftBrace)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::MatchStmt,
                        .reason = EParseErrorReason::MissingBody,
                    });
                    return syntax::stmt::Match{
                        std::move(expr), {}, {}, location
                    };
                }
                while (!m_stream.match(lexeme::rightBrace)) {
                    if (m_stream.match(lexeme::anonymousIdentifier)) {
                        if (!m_stream.match(lexeme::equalGreater)) {
                            collect({
                                .location = m_tracker.current(),
                                .context = EParseErrorContext::MatchCaseDefaultStmt,
                                .reason = EParseErrorReason::MissingSymbol,
                            });
                        }
                        defaultStmt = *handleStmt().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::MatchCaseDefaultStmt,
                                    .reason = EParseErrorReason::MissingStmt,
                                });
                                return {};
                            }
                        );
                        continue;
                    }

                    auto caseCoords = m_stream.peek().location();
                    auto value = *handleExpr().or_else(
                        [this](auto&& error) -> ParseResult {
                            collect(error);
                            return {};
                        }
                    );
                    auto cond =
                        match(lexeme::when)(m_stream, m_tracker)
                        .and_then(
                            [this](auto&&) -> ParseResult {
                                return handleExpr().or_else(
                                    [this](auto&& error) -> ParseResult {
                                        collect(error);
                                        collect({
                                            .location = m_tracker.current(),
                                            .context = EParseErrorContext::MatchCaseStmt,
                                            .reason = EParseErrorReason::MissingExpr,
                                        });
                                        return {};
                                    });
                            }
                        ).value_or({});
                    if (!m_stream.match(lexeme::equalGreater)) {
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::MatchCaseStmt,
                            .reason = EParseErrorReason::MissingSymbol,
                        });
                    }
                    auto stmt = *handleStmt().or_else(
                        [this](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_tracker.current(),
                                .context = EParseErrorContext::MatchCaseStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        });
                    cases.emplace_back(syntax::stmt::MatchCase{
                        std::move(value), std::move(cond),
                        std::move(stmt), std::move(caseCoords)
                    });
                }

                if (m_stream.current().lexeme() != lexeme::rightBrace) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::MatchStmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }
                return syntax::stmt::Match{
                    std::move(expr), std::move(cases), std::move(defaultStmt),
                    location
                };
            }
        );
    }

    auto Parse::handleBlockStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::leftBrace)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();

                Vec<syntax::Node> statements;
                while (!m_stream.match(lexeme::rightBrace)) {
                    statements.push_back(*parseStmt()
                        .or_else([this](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_tracker.current(),
                                .context = EParseErrorContext::BlockStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        })
                    );
                }

                if (m_stream.current().lexeme() != lexeme::leftBrace) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::BlockStmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Block{std::move(statements), location};
            }
        );
    }

    auto Parse::handleDeferStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::defer)(m_stream, m_tracker)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                return syntax::stmt::Defer{
                    *handleStmt().or_else(
                        [this, &location](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_tracker.current(),
                                .context = EParseErrorContext::DeferStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        }),
                    location
                };
            });
    }


    auto Parse::handlePrefaceStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::preface)(m_stream, m_tracker)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                return syntax::stmt::Preface{
                    *handleStmt().or_else(
                        [this, &location](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_tracker.current(),
                                .context = EParseErrorContext::PrefaceStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        }),
                    location
                };
            });
    }

    auto Parse::handleYieldStmt() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::yield)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();

                auto yieldStmt = syntax::stmt::Yield{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_tracker.current(),
                            .context = EParseErrorContext::YieldStmt,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return {};
                    }),
                    location,
                };

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return yieldStmt;
            }
        );
    }
}
