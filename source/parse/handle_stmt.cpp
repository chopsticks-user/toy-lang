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
        // todo: last to be considered to avoid ambiguity with records
        if (auto blockStmt = handleBlockStmt(); blockStmt) {
            return blockStmt;
        }
        return {};
    }

    auto Parse::handleLetStmt() -> ParseResult {
        return match(Let)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                auto decl = *parseDecl().or_else([this](auto&& error) -> ParseResult {
                    m_panic.collect(error);
                    m_panic.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Stmt,
                        .reason = Error::Reason::MissingDecl,
                    });
                    return {};
                });

                auto initializer =
                    *match(Equal)(m_context, m_stream, m_panic)
                     .and_then([this](auto) -> ParseResult {
                         return *parseExpr().or_else([this](auto&& error) -> ParseResult {
                             m_panic.collect(error);
                             return {};
                         });
                     }).or_else([this](auto) -> ParseResult { return {}; });

                if (!m_stream.match(Semicolon)) {
                    m_panic.collect({
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
        return match(Return)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                auto returnStmt = syntax::stmt::Return{
                    *parseExpr().or_else([this](auto&& error) -> ParseResult {
                        m_panic.collect(error);
                        return {};
                    }),
                    std::move(coords),
                };

                if (!m_stream.match(Semicolon)) {
                    m_panic.collect({
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
        return {};
    }

    auto Parse::handleLoopStmt() -> ParseResult {
        return {};
    }

    auto Parse::handleMatchStmt() -> ParseResult {
        return {};
    }

    auto Parse::handleConditionalStmt() -> ParseResult {
        return {};
    }

    auto Parse::handleBlockStmt() -> ParseResult {
        return match(LeftBrace)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                Vec<syntax::Node> statements;
                while (!m_stream.match(RightBrace)) {
                    statements.push_back(*parseStmt()
                        .or_else([this](auto&& error) -> ParseResult {
                            m_panic.collect(error);
                            m_panic.collect({
                                .location = m_stream.current().coords(),
                                .context = Error::Context::BlockStmt,
                                .reason = Error::Reason::MissingStmt,
                            });
                            return {};
                        })
                    );
                }

                if (m_stream.current().type() != LeftBrace) {
                    m_panic.collect({
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
        return {};
    }

    auto Parse::handlePrefaceStmt() -> ParseResult {
        return {};
    }

    auto Parse::handleYieldStmt() -> ParseResult {
        return match(Yield)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                return syntax::stmt::Yield{
                    *handleExpr().or_else([this](auto&& error) -> ParseResult {
                        m_panic.collect(error);
                        return {};
                    }),
                    tokens.front().coords()
                };
            }
        );
    }
}
