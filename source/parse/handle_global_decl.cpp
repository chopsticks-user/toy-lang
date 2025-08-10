#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleTranslationUnit() -> ParseResult {
        auto context = enter(Context::TranslationUnit);

        auto moduleDecl = handleModuleDecl()
            .value_or(syntax::RequiredButMissing{});
        if (context.emitIfNodeEmpty(moduleDecl, Reason::MissingDecl)) {
            return defaultError();
        }

        syntax::Node importGroup;
        {
            auto importGroupContext = enter(Context::ImportDeclGroup);

            Vec<syntax::Node> imports;
            while (context.stream().peek().lexeme() != lexeme::invalid) {
                auto importDecl = handleImportDecl();
                if (!importDecl) {
                    break;
                }
                imports.push_back(std::move(*importDecl));
            }
            if (!imports.empty()) {
                importGroup = syntax::global::ImportDeclGroup{
                    std::move(imports), importGroupContext.location()
                };
            }
        }


        Vec<syntax::Node> definitions;
        while (context.stream().peek().lexeme() != lexeme::invalid) {
            auto const visibility =
                context.stream().match(lexeme::pub, lexeme::prv)
                    ? context.stream().current()
                    : createDefaultVisibility();

            if (auto fnDef = handleFunctionDef(visibility); fnDef) {
                definitions.push_back(std::move(*fnDef));
            }
        }

        return syntax::TranslationUnit{
            context.filepath(), std::move(moduleDecl),
            std::move(importGroup), std::move(definitions)
        };
    }

    auto Parse::handleModuleDecl() -> ParseResult {
        auto context = enter(Context::ModuleDecl);

        if (!context.stream().match(lexeme::module_)) {
            return defaultError();
        }

        auto path = handleIdentifierLiteral()
            .value_or(syntax::RequiredButMissing{});
        if (context.emitIfNodeEmpty(path, Reason::MissingId)) {
            return defaultError();
        }

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::global::ModuleDecl{
            std::move(path), context.location()
        };
    }


    auto Parse::handleImportDecl() -> ParseResult {
        auto context = enter(Context::ImportDecl);

        if (!context.stream().match(lexeme::import_)) {
            return defaultError();
        }

        auto path_or_alias = handleIdentifierLiteral()
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(path_or_alias, Reason::MissingId);

        syntax::Node path;
        if (context.stream().match(lexeme::equal)) {
            path = handleIdentifierLiteral()
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(path_or_alias, Reason::MissingExpr);
        }

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::global::ImportDecl{
            std::move(path_or_alias), std::move(path), context.location()
        };
    }
}
