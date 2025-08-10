#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleFunctionDef(token::Token const& visibility) -> ParseResult {
        auto context = enter(Context::Function);

        auto prototype = handleFunctionPrototype().value_or({});
        if (syntax::isEmptyNode(prototype)) {
            return defaultError();
        }

        auto body = handleBlockStmt().value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(body, Reason::MissingBody);
        return syntax::global::Function{
            visibility.lexeme(), std::move(prototype),
            std::move(body), visibility.location()
        };
    }

    auto Parse::handleFunctionPrototype() -> ParseResult {
        auto context = enter(Context::FunctionPrototype);

        if (!context.stream().match(lexeme::fn)) {
            return defaultError();
        }

        auto genericDecl = handleGenericParamsDecl().value_or({});
        Str name;
        if (!context.emitIfLexemeNotPresent(
            lexeme::identifier, Reason::MissingId)) {
            name = context.stream().current().str();
        }
        context.emitIfLexemeNotPresent(lexeme::colon, Reason::MissingDecl);

        auto paramsDecl = handleTupleDecl()
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(paramsDecl, Reason::MissingDecl);
        context.emitIfLexemeNotPresent(
            lexeme::minusGreater, Reason::MissingSymbol
        );
        auto returnsDecl = handleTupleDecl().value_or({});

        return syntax::global::FunctionPrototype{
            std::move(genericDecl), std::move(name), std::move(paramsDecl),
            std::move(returnsDecl), context.location()
        };
    }
}
