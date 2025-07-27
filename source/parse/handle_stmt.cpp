#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleStmt() -> ParseResult {
        return handleLetStmt();
    }

    auto Parse::handleLetStmt() -> ParseResult {
        return match(Let)(m_context, m_stream, m_panic).and_then(
            [this](auto const& tokens) -> ParseResult {
                [[maybe_unused]] auto coords = tokens.front().coords();

                if (!m_stream.match(Semicolon)) {
                    // todo: collect errors
                }

                return {};
            }
        );
    }

    auto Parse::handleReturnStmt() -> ParseResult {
        return {};
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

                if (!m_stream.match(RightBrace)) {
                    // todo: collect errors
                }

                return {};
            }
        );
    }
}
