#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleDecl(Context context) -> Opt<syntax::Node> {
        if (auto tupleDecl = handleTupleDecl(
            Context::enter(EContext::TupleDecl, context)); tupleDecl) {
            return tupleDecl;
        }
        if (auto idDecl = handleIdentifierDecl(
            Context::enter(EContext::IdDecl, context)); idDecl) {
            return idDecl;
        }
        return {};
    }

    auto handleIdentifierDecl(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::identifier, lexeme::anonymous)) {
            return {};
        }

        auto name = context.stream().current().str();
        if (!context.stream().match(lexeme::colon)) {
            return syntax::decl::Identifier{
                std::move(name), {}, context.location()
            };
        }

        auto type = handleType(Context::enter(EContext::Type, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(type, EReason::MissingTypeExpr);
        return syntax::decl::Identifier{
            std::move(name), std::move(type), context.location()
        };
    }

    auto handleTupleDecl(Context context) -> Opt<syntax::Node> {
        static const SynchronizingSet synchronizingSet = {
            lexeme::comma, lexeme::rightParen
        };

        if (!context.stream().match(lexeme::leftParen)) {
            return {};
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::decl::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> decls;
        do {
            auto decl = handleIdentifierDecl(
                Context::enter(EContext::IdDecl, context)
            ).value_or(syntax::RequiredButMissing{});
            if (context.emitIfNodeMissing(decl, EReason::MissingDecl)) {
                context.synchronize(synchronizingSet);
            }
            decls.push_back(std::move(decl));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, EReason::MissingEnclosingSymbol
        );
        return syntax::decl::Tuple{std::move(decls), context.location()};
    }

    auto handleGenericParamsDecl(Context context) -> Opt<syntax::Node> {
        static const SynchronizingSet synchronizingSet = {
            lexeme::comma, lexeme::greater
        };

        if (!context.stream().match(lexeme::less)) {
            return {};
        }
        if (context.stream().match(lexeme::greater)) {
            return syntax::decl::GenericParameters{{}, context.location()};
        }

        Vec<syntax::Node> types;
        do {
            if (!context.emitIfLexemeNotPresent(
                lexeme::userDefinedType, EReason::MissingTypeId
            )) {
                auto token = context.stream().current();
                types.emplace_back(syntax::decl::GenericIdentifier{
                    Str(token.str()), token.location()
                });
            }
            else {
                context.synchronize(synchronizingSet);
                types.emplace_back(syntax::RequiredButMissing{});
            }
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::greater, EReason::MissingEnclosingSymbol
        );
        return syntax::decl::GenericParameters{
            std::move(types), context.location()
        };
    }
}
