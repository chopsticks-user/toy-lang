#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleDecl() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return handleTupleDecl().or_else([this](auto&& error) -> ParseResult {
            collect(error);
            return handleIdentifierDecl();
        });
    }

    auto Parse::handleIdentifierDecl() -> ParseResult {
        TLC_SCOPE_REPORTER();
        auto const location = m_tracker.scopedLocation();

        return match(lexeme::identifier)(m_stream, m_tracker).and_then(
            [&](auto const& tokens) -> ParseResult {
                auto name = tokens.front().str();

                if (!m_stream.match(lexeme::colon)) {
                    return syntax::decl::Identifier{
                        tokens.front().str(), {}, *location
                    };
                }

                return handleType().and_then([&](auto const& type)
                    -> ParseResult {
                        return syntax::decl::Identifier{
                            name, type, *location
                        };
                    }).or_else([&]([[maybe_unused]] auto&& error)
                    -> ParseResult {
                        collect(error);
                        return syntax::decl::Identifier{
                            name, {}, *location
                        };
                    });
            }
        ).or_else([this](auto) -> ParseResult {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::IdDecl,
                .reason = EParseErrorReason::MissingId
            });
            return {};
        });
    }

    auto Parse::handleTupleDecl() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return match(lexeme::leftParen)(m_stream, m_tracker).and_then(
            [this](auto const& tokens) -> ParseResult {
                auto location = tokens.front().location();
                Vec<syntax::Node> decls;

                if (m_stream.match(lexeme::rightParen)) {
                    return syntax::decl::Tuple{std::move(decls), location};
                }

                do {
                    decls.push_back(*handleDecl().or_else(
                            [this](auto&& error) -> ParseResult {
                                collect(error).collect({
                                    .location = m_tracker.current(),
                                    .context = EParseErrorContext::Tuple,
                                    .reason = EParseErrorReason::MissingDecl,
                                });
                                return syntax::Node{};
                            }
                        )
                    );
                }
                while (m_stream.match(lexeme::comma));

                if (!m_stream.match(lexeme::rightParen)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::Tuple,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::decl::Tuple{std::move(decls), location};
            }
        );
    }
}
