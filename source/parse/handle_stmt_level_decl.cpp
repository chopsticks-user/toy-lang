#include "parse.hpp"

namespace tlc::parse {
    using enum token::EToken;

    auto Parse::handleStmtLevelDecl() -> ParseResult {
        return handleTupleDecl().or_else([this](auto const&) -> ParseResult {
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
                    }).or_else([this, constant, id](auto const& error)
                    -> ParseResult {
                        // todo: collect errors
                        return syntax::decl::Identifier{
                            constant, id, {}, popCoords()
                        };
                    });
            });
    }

    auto Parse::handleTupleDecl() -> ParseResult {
        // empty tuples are not allowed
    }
}
