#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleDecl(Context context) -> Opt<syntax::Node> {
        if (auto tupleDecl = handleTupleDecl(
            Context::enter(Context::TupleDecl, context)); tupleDecl) {
            return tupleDecl;
        }
        if (auto idDecl = handleIdentifierDecl(
            Context::enter(Context::IdDecl, context)); idDecl) {
            return idDecl;
        }
        return {};
    }

    auto handleIdentifierDecl(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::identifier)) {
            return {};
        }

        auto name = context.stream().current().str();
        if (!context.stream().match(lexeme::colon)) {
            return syntax::decl::Identifier{
                std::move(name), {}, context.location()
            };
        }

        auto type = handleType(Context::enter(Context::Type, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(type, Reason::MissingType);
        return syntax::decl::Identifier{
            std::move(name), std::move(type), context.location()
        };
    }

    auto handleTupleDecl(Context context) -> Opt<syntax::Node> {
        if (context.backtrackIf(!context.stream().match(lexeme::leftParen))) {
            return {};
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::decl::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> decls;
        do {
            auto decl = handleDecl(Context::enter(Context::Decl, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(decl, Reason::MissingDecl);
            decls.push_back(std::move(decl));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, Reason::MissingEnclosingSymbol
        );
        return syntax::decl::Tuple{std::move(decls), context.location()};
    }

    auto handleGenericParamsDecl(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::less)) {
            return {};
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
