#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleDecl() -> ParseResult {
        if (auto tupleDecl = handleTupleDecl(); tupleDecl) {
            return tupleDecl;
        }
        if (auto idDecl = handleIdentifierDecl(); idDecl) {
            return idDecl;
        }
        return defaultError();
    }

    auto Parse::handleIdentifierDecl() -> ParseResult {
        auto context = enter(Context::IdDecl);

        if (!context.stream().match(lexeme::identifier)) {
            return defaultError();
        }

        auto name = context.stream().current().str();
        if (!context.stream().match(lexeme::colon)) {
            return syntax::decl::Identifier{
                std::move(name), {}, context.location()
            };
        }

        auto type = handleType().value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(type, Reason::MissingType);
        return syntax::decl::Identifier{
            std::move(name), std::move(type), context.location()
        };
    }

    auto Parse::handleTupleDecl() -> ParseResult {
        auto context = enter(Context::TupleDecl);

        if (context.backtrackIf(!context.stream().match(lexeme::leftParen))) {
            return defaultError();
        }
        if (m_stream.match(lexeme::rightParen)) {
            return syntax::decl::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> decls;
        do {
            auto decl = handleDecl().value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(decl, Reason::MissingDecl);
            decls.push_back(std::move(decl));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, Reason::MissingEnclosingSymbol
        );
        return syntax::decl::Tuple{std::move(decls), context.location()};
    }

    auto Parse::handleGenericParamsDecl() -> ParseResult {
        auto context = enter(Context::GenericParamsDecl);

        if (!context.stream().match(lexeme::less)) {
            return defaultError();
        }
        if (context.stream().match(lexeme::greater)) {
            return syntax::decl::GenericParameters{{}, context.location()};
        }

        Vec<syntax::Node> types;
        do {
            if (!context.emitIfLexemeNotPresent(
                lexeme::userDefinedType, Reason::MissingId
            )) {
                auto token = context.stream().current();
                types.emplace_back(syntax::decl::GenericIdentifier{
                    Str(token.str()), token.location()
                });
            }
            else {
                types.emplace_back(syntax::RequiredButMissing{});
            }
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::greater, Reason::MissingEnclosingSymbol
        );
        return syntax::decl::GenericParameters{
            std::move(types), context.location()
        };
    }
}
