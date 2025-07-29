#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

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
        return match(Let)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                auto decl = *parseDecl().or_else([this](auto&& error) -> ParseResult {
                    m_collector.collect(error);
                    m_collector.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Stmt,
                        .reason = Error::Reason::MissingDecl,
                    });
                    return {};
                });

                auto initializer =
                    *match(Equal)(m_stream, m_tracker, m_collector)
                     .and_then([this](auto) -> ParseResult {
                         return *parseExpr().or_else([this](auto&& error) -> ParseResult {
                             m_collector.collect(error);
                             return {};
                         });
                     }).or_else([this](auto) -> ParseResult { return {}; });

                if (!m_stream.match(Semicolon)) {
                    m_collector.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Stmt,
                        .reason = Error::Reason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Let{
                    std::move(decl), std::move(initializer), std::move(coords)
                };
            }
        );
    }

    auto Parse::handleReturnStmt() -> ParseResult {
        return match(Return)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                auto returnStmt = syntax::stmt::Return{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        m_collector.collect(error);
                        return {};
                    }),
                    std::move(coords),
                };

                if (!m_stream.match(Semicolon)) {
                    m_collector.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Stmt,
                        .reason = Error::Reason::MissingEnclosingSymbol,
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
                    auto const op = m_stream.current().type();
                    expr = syntax::stmt::Assign{
                        expr, *handleExpr().or_else(
                            [this](auto&& error) -> ParseResult {
                                m_collector.collect(error);
                                m_collector.collect({
                                    .location = m_stream.current().coords(),
                                    .context = Error::Context::AssignStmt,
                                    .reason = Error::Reason::MissingExpr,
                                });
                                return {};
                            }
                        ),
                        op, popCoords()
                    };
                }
                else if (m_stream.match(EqualGreater)) {
                    return syntax::stmt::Cond{
                        expr, *handleStmt().or_else(
                            [this](auto&& error) -> ParseResult {
                                m_collector.collect(error);
                                m_collector.collect({
                                    .location = m_stream.current().coords(),
                                    .context = Error::Context::CondStmt,
                                    .reason = Error::Reason::MissingStmt,
                                });
                                return {};
                            }
                        ),
                        popCoords()
                    };
                }
                else {
                    expr = syntax::stmt::Expr{expr, popCoords()};
                }

                if (!m_stream.match(Semicolon)) {
                    m_collector.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Stmt,
                        .reason = Error::Reason::MissingEnclosingSymbol,
                    });
                }
                return expr;
            });
    }

    auto Parse::handleLoopStmt() -> ParseResult {
        return match(For)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                return {};
            }
        );
    }

    auto Parse::handleMatchStmt() -> ParseResult {
        return match(Match)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();

                return {};
            }
        );
    }

    auto Parse::handleBlockStmt() -> ParseResult {
        return match(LeftBrace)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                Vec<syntax::Node> statements;
                while (!m_stream.match(RightBrace)) {
                    statements.push_back(*parseStmt()
                        .or_else([this](auto&& error) -> ParseResult {
                            m_collector.collect(error);
                            m_collector.collect({
                                .location = m_stream.current().coords(),
                                .context = Error::Context::BlockStmt,
                                .reason = Error::Reason::MissingStmt,
                            });
                            return {};
                        })
                    );
                }

                if (m_stream.current().type() != LeftBrace) {
                    m_collector.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::BlockStmt,
                        .reason = Error::Reason::MissingEnclosingSymbol,
                    });
                }

                return syntax::stmt::Block{
                    std::move(statements), std::move(coords)
                };
            }
        );
    }

    auto Parse::handleDeferStmt() -> ParseResult {
        return match(Defer)(m_stream, m_tracker, m_collector)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                return syntax::stmt::Defer{
                    *handleStmt().or_else(
                        [this, &coords](auto&& error) -> ParseResult {
                            m_collector.collect(error);
                            m_collector.collect({
                                .location = m_stream.current().coords(),
                                .context = Error::Context::DeferStmt,
                                .reason = Error::Reason::MissingStmt,
                            });
                            return syntax::stmt::Defer{
                                {}, std::move(coords)
                            };
                        }),
                    std::move(coords)
                };
            });
    }


    auto Parse::handlePrefaceStmt() -> ParseResult {
        return match(Preface)(m_stream, m_tracker, m_collector)
            .and_then([this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                return syntax::stmt::Preface{
                    *handleStmt().or_else(
                        [this, &coords](auto&& error) -> ParseResult {
                            m_collector.collect(error);
                            m_collector.collect({
                                .location = m_stream.current().coords(),
                                .context = Error::Context::PrefaceStmt,
                                .reason = Error::Reason::MissingStmt,
                            });
                            return syntax::stmt::Preface{
                                {}, std::move(coords)
                            };
                        }),
                    std::move(coords)
                };
            });
    }

    auto Parse::handleYieldStmt() -> ParseResult {
        return match(Yield)(m_stream, m_tracker, m_collector).and_then(
            [this](auto const& tokens) -> ParseResult {
                return syntax::stmt::Yield{
                    *handleExpr().or_else([this](auto&& error) -> ParseResult {
                        m_collector.collect(error);
                        return {};
                    }),
                    tokens.front().coords()
                };
            }
        );
    }
}
