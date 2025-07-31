#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleStmtLevelDecl() -> ParseResult {
        return handleTupleDecl().or_else([this](auto&& error) -> ParseResult {
            collect(error);
            return handleIdentifierDecl();
        });
    }

    auto Parse::handleIdentifierDecl() -> ParseResult {
        pushCoords();
        auto const constant = !m_stream.match(Dollar);

        return match(Identifier)(m_stream, m_tracker).and_then(
            [this, constant](auto const& tokens) -> ParseResult {
                auto name = tokens.front().str();

                if (!m_stream.match(Colon)) {
                    return syntax::decl::Identifier{
                        constant, tokens.front().str(), {}, popCoords()
                    };
                }

                return handleType().and_then([this, constant, name](auto const& type)
                    -> ParseResult {
                        return syntax::decl::Identifier{
                            constant, name, type, popCoords()
                        };
                    }).or_else([this, constant, name]([[maybe_unused]] auto&& error)
                    -> ParseResult {
                        collect(error);
                        return syntax::decl::Identifier{
                            constant, name, {}, popCoords()
                        };
                    });
            }
        ).or_else([this](auto) -> ParseResult {
            collect({
                .location = m_stream.current().coords(),
                .context = EParseErrorContext::IdDecl,
                .reason = EParseErrorReason::MissingId
            });
            return {};
        });
    }

    auto Parse::handleTupleDecl() -> ParseResult {
        return match(LeftParen)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto coords = tokens.front().coords();
                Vec<syntax::Node> decls;

                if (m_stream.match(RightParen)) {
                    return syntax::decl::Tuple{
                        std::move(decls), std::move(coords)
                    };
                }

                do {
                    decls.push_back(*handleStmtLevelDecl().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error).collect({
                                    .location = m_stream.current().coords(),
                                    .context = EParseErrorContext::Tuple,
                                    .reason = EParseErrorReason::MissingDecl,
                                });
                                return syntax::Node{};
                            }
                        )
                    );
                }
                while (m_stream.match(Comma));

                if (!m_stream.match(RightParen)) {
                    collect({
                        .location = m_stream.current().coords(),
                        .context = EParseErrorContext::Tuple,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::decl::Tuple{
                    std::move(decls),
                    std::move(coords)
                };
            }
        );
    }
}
