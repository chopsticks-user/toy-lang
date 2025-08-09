#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleDecl() -> ParseResult {
        TLC_SCOPE_REPORTER();
        return handleTupleDecl().or_else([this](auto&&) -> ParseResult {
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
                            Str(name), {}, *location
                        };
                    });
            }
        );
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

    auto Parse::handleGenericParamsDecl() -> ParseResult {
        return match(lexeme::less)(m_stream, m_tracker).and_then(
            [this](auto const&) -> ParseResult {
                auto const location = m_tracker.current();

                if (m_stream.match(lexeme::greater)) {
                    return syntax::decl::GenericParameters{{}, location};
                }

                Vec<syntax::Node> types;
                do {
                    types.push_back(
                        *match(lexeme::userDefinedType)
                         (m_stream, m_tracker)
                         .and_then([&](auto&& typeId) -> ParseResult {
                             return syntax::decl::GenericIdentifier{
                                 Str(typeId.front().str()),
                                 m_tracker.current()
                             };
                         })
                         .or_else(
                             [this](auto&& error) -> ParseResult {
                                 collect(error).collect({
                                     .location = m_tracker.current(),
                                     .context = EParseErrorContext::GenericParamsDecl,
                                     .reason = EParseErrorReason::MissingId,
                                 });
                                 return {};
                             }
                         )
                    );
                }
                while (m_stream.match(lexeme::comma));

                if (!m_stream.match(lexeme::greater)) {
                    collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::GenericParamsDecl,
                        .reason = EParseErrorReason::MissingEnclosingSymbol,
                    });
                }

                return syntax::decl::GenericParameters{
                    std::move(types), location
                };
            });
    }
}
