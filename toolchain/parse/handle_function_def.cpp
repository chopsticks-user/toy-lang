#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleFunctionDef(Context context) -> Opt<syntax::Node> {
        auto prototype = handleFunctionPrototype(
                Context::enter(EContext::FunctionPrototype, context))
            .value_or({});
        if (syntax::empty(prototype)) {
            return {};
        }

        auto body = handleBlockStmt(Context::enter(EContext::BlockStmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(body, EReason::MissingBody);
        return syntax::global::Function{
            context.visibility().lexeme(), std::move(prototype),
            std::move(body), context.visibility().location()
        };
    }

    auto handleFunctionPrototype(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::fn)) {
            return {};
        }

        auto genericDecl = handleGenericParamsDecl(
            Context::enter(EContext::GenericParamsDecl, context)).value_or({});
        Str name;
        if (!context.emitIfLexemeNotPresent(
            lexeme::identifier, EReason::MissingId)) {
            name = context.stream().current().str();
        }
        context.emitIfLexemeNotPresent(lexeme::colon, EReason::MissingDecl);

        auto paramsDecl = handleTupleDecl(Context::enter(EContext::TupleDecl, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(paramsDecl, EReason::MissingDecl);
        context.emitIfLexemeNotPresent(
            lexeme::minusGreater, EReason::MissingSymbol
        );
        auto returnsDecl = handleTupleDecl(Context::enter(EContext::TupleDecl, context))
            .value_or({});

        return syntax::global::FunctionPrototype{
            std::move(genericDecl), std::move(name), std::move(paramsDecl),
            std::move(returnsDecl), context.location()
        };
    }
}
