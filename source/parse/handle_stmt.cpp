#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleStmt() -> ParseResult {
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
        return match(lexeme::let)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                auto decl = *parseDecl().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_stream.current().coords(),
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
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Let{
                    std::move(decl), std::move(initializer), std::move(coords)
                };
            }
        );
    }

    auto Parse::handleReturnStmt() -> ParseResult {
        return match(lexeme::return_)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                auto returnStmt = syntax::stmt::Return{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        collect(error);
                        return {};
                    }),
                    std::move(coords),
                };

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return returnStmt;
            }
        );
    }

    auto Parse::handleExprPrefixedStmt() -> ParseResult {
        pushCoords();
        return handleExpr().and_then([this](auto&& expr)
            -> ParseResult {
                if (m_stream.match(syntax::isAssignmentOperator)) {
                    auto const op = m_stream.current().lexeme();
                    expr = syntax::stmt::Assign{
                        expr, *handleExpr().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::AssignStmt,
                                    .reason = EParseErrorReason::MissingExpr,
                                });
                                return {};
                            }
                        ),
                        op, popCoords()
                    };
                }
                else if (m_stream.match(lexeme::equalGreater)) {
                    expr = syntax::stmt::Conditional{
                        expr, *handleStmt().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::CondStmt,
                                    .reason = EParseErrorReason::MissingStmt,
                                });
                                return {};
                            }
                        ),
                        popCoords()
                    };
                }
                else {
                    expr = syntax::stmt::Expression{expr, popCoords()};
                }

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }
                return expr;
            });
    }

    auto Parse::handleLoopStmt() -> ParseResult {
        return match(lexeme::for_)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                auto decl = *handleStmtLevelDecl().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_stream.current().coords(),
                            .context = EParseErrorContext::LoopStmt,
                            .reason = EParseErrorReason::MissingDecl,
                        });
                        return {};
                    }
                );
                if (!m_stream.match(lexeme::in)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::LoopStmt,
                        .reason = EParseErrorReason::MissingKeyword,
                    });
                }
                auto range = *handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_stream.current().coords(),
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
                            .location = m_stream.current().coords(),
                            .context = EParseErrorContext::LoopStmt,
                            .reason = EParseErrorReason::MissingStmt,
                        });
                        return {};
                    }
                );
                return syntax::stmt::Loop{
                    std::move(decl), std::move(range),
                    std::move(body), std::move(coords)
                };
            }
        );
    }

    auto Parse::handleMatchStmt() -> ParseResult {
        return match(lexeme::match)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                auto expr = *handleExpr().or_else(
                    [this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_stream.current().coords(),
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
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::MatchStmt,
                        .reason = EParseErrorReason::MissingBody,
                    });
                    return syntax::stmt::Match{
                        std::move(expr), {}, {}, std::move(coords)
                    };
                }
                while (!m_stream.match(lexeme::rightBrace)) {
                    if (m_stream.match(lexeme::anonymousIdentifier)) {
                        if (!m_stream.match(lexeme::equalGreater)) {
                            collect({
                                .location = m_stream.current().coords(),
                                .context = EParseErrorContext::MatchCaseDefaultStmt,
                                .reason = EParseErrorReason::MissingSymbol,
                            });
                        }
                        defaultStmt = *handleStmt().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error);
                                collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::MatchCaseDefaultStmt,
                                    .reason = EParseErrorReason::MissingStmt,
                                });
                                return {};
                            }
                        );
                        continue;
                    }

                    auto caseCoords = m_stream.peek().coords();
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
                                            .location = m_stream.current().coords(),
                                            .context = EParseErrorContext::MatchCaseStmt,
                                            .reason = EParseErrorReason::MissingExpr,
                                        });
                                        return {};
                                    });
                            }
                        ).value_or({});
                    if (!m_stream.match(lexeme::equalGreater)) {
                        collect({
                            .location = m_stream.current().coords(),
                            .context = EParseErrorContext::MatchCaseStmt,
                            .reason = EParseErrorReason::MissingSymbol,
                        });
                    }
                    auto stmt = *handleStmt().or_else(
                        [this](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_stream.current().coords(),
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
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::MatchStmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }
                return syntax::stmt::Match{
                    std::move(expr), std::move(cases), std::move(defaultStmt),
                    std::move(coords)
                };
            }
        );
    }

    auto Parse::handleBlockStmt() -> ParseResult {
        return match(lexeme::leftBrace)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                Vec<syntax::Node> statements;
                while (!m_stream.match(lexeme::rightBrace)) {
                    statements.push_back(*parseStmt()
                        .or_else([this](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_stream.current().coords(),
                                .context = EParseErrorContext::BlockStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        })
                    );
                }

                if (m_stream.current().lexeme() != lexeme::leftBrace) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::BlockStmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Block{
                    std::move(statements), std::move(coords)
                };
            }
        );
    }

    auto Parse::handleDeferStmt() -> ParseResult {
        return match(lexeme::defer)(m_stream, m_tracker)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                return syntax::stmt::Defer{
                    *handleStmt().or_else(
                        [this, &coords](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_stream.current().coords(),
                                .context = EParseErrorContext::DeferStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        }),
                    std::move(coords)
                };
            });
    }


    auto Parse::handlePrefaceStmt() -> ParseResult {
        return match(lexeme::preface)(m_stream, m_tracker)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                return syntax::stmt::Preface{
                    *handleStmt().or_else(
                        [this, &coords](auto&& error) -> ParseResult {
                            collect(error);
                            collect({
                                .location = m_stream.current().coords(),
                                .context = EParseErrorContext::PrefaceStmt,
                                .reason = EParseErrorReason::MissingStmt,
                            });
                            return {};
                        }),
                    std::move(coords)
                };
            });
    }

    auto Parse::handleYieldStmt() -> ParseResult {
        return match(lexeme::yield)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                auto yieldStmt = syntax::stmt::Yield{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        collect(error);
                        collect({
                            .location = m_stream.current().coords(),
                            .context = EParseErrorContext::YieldStmt,
                            .reason = EParseErrorReason::MissingExpr,
                        });
                        return {};
                    }),
                    std::move(coords),
                };

                if (!m_stream.match(lexeme::semicolon)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Stmt,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return yieldStmt;
            }
        );
    }
}
