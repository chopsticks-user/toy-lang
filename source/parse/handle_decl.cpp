#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleStmtLevelDecl() -> ParseResult {
        return handleTupleDecl().or_else([this](auto&& error) -> ParseResult {
            m_panic.collect(error);
            return handleIdentifierDecl();
        });
    }

    auto Parse::handleIdentifierDecl() -> ParseResult {
        pushCoords();
        auto const constant = !m_stream.match(Dollar);

        return handleIdentifierLiteral().and_then([this, constant](auto const& id)
            -> ParseResult {
                if (!m_stream.match(Colon)) {
                    return syntax::decl::Identifier{
                        constant, id, {}, popCoords()
                    };
                }

                return handleType().and_then([this, constant, id](auto const& type)
                    -> ParseResult {
                        return syntax::decl::Identifier{
                            constant, id, type, popCoords()
                        };
                    }).or_else([this, constant, id]([[maybe_unused]] auto&& error)
                    -> ParseResult {
                        m_panic.collect(error);
                        return syntax::decl::Identifier{
                            constant, id, {}, popCoords()
                        };
                    });
            });
    }

    auto Parse::handleTupleDecl() -> ParseResult {
        return match(LeftParen)(m_context, m_stream, m_panic).and_then(
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
                                m_panic.collect(error);
                                m_panic.collect({
                                    .location = m_stream.current().coords(),
                                    .context = Error::Context::Tuple,
                                    .reason = Error::Reason::MissingDecl,
                                });
                                return syntax::Node{};
                            }
                        )
                    );
                }
                while (m_stream.match(Comma));

                if (!m_stream.match(RightParen)) {
                    m_panic.collect({
                        .location = m_stream.current().coords(),
                        .context = Error::Context::Tuple,
                        .reason = Error::Reason::MissingEnclosingSymbol,
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
