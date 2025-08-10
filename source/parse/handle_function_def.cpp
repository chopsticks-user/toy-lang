#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleFunctionDef(Context context) -> Opt<syntax::Node> {
        auto prototype = handleFunctionPrototype(
                Context::enter(Context::FunctionPrototype, context))
            .value_or({});
        if (syntax::isEmptyNode(prototype)) {
            return {};
        }

        auto body = handleBlockStmt(Context::enter(Context::BlockStmt, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(body, Reason::MissingBody);
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
            Context::enter(Context::GenericParamsDecl, context)).value_or({});
        Str name;
        if (!context.emitIfLexemeNotPresent(
            lexeme::identifier, Reason::MissingId)) {
            name = context.stream().current().str();
        }
        context.emitIfLexemeNotPresent(lexeme::colon, Reason::MissingDecl);

        auto paramsDecl = handleTupleDecl(Context::enter(Context::TupleDecl, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(paramsDecl, Reason::MissingDecl);
        context.emitIfLexemeNotPresent(
            lexeme::minusGreater, Reason::MissingSymbol
        );
        auto returnsDecl = handleTupleDecl(Context::enter(Context::TupleDecl, context))
            .value_or({});

        return syntax::global::FunctionPrototype{
            std::move(genericDecl), std::move(name), std::move(paramsDecl),
            std::move(returnsDecl), context.location()
        };
    }
}
