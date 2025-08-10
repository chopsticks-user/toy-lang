#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto handleTranslationUnit(Context context) -> Opt<syntax::Node> {
        auto moduleDecl =
            handleModuleDecl(Context::enter(Context::ModuleDecl, context))
            .value_or(syntax::RequiredButMissing{});
        if (context.emitIfNodeEmpty(moduleDecl, Reason::MissingDecl)) {
            return {};
        }

        syntax::Node importGroup;
        {
            auto importGroupContext = Context::enter(Context::ImportDeclGroup, context);

            Vec<syntax::Node> imports;
            while (context.stream().peek().lexeme() != lexeme::invalid) {
                auto importDecl = handleImportDecl(
                    Context::enter(Context::ImportDecl, context));
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
            Opt<token::Token> visibility;
            if (context.stream().match(lexeme::pub, lexeme::prv)) {
                visibility = context.stream().current();
            }

            if (auto fnDef = handleFunctionDef(
                Context::enter(Context::Function, context,
                               {}, std::move(visibility))); fnDef) {
                definitions.push_back(std::move(*fnDef));
            }
        }

        return syntax::TranslationUnit{
            context.filepath(), std::move(moduleDecl),
            std::move(importGroup), std::move(definitions)
        };
    }

    auto handleModuleDecl(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::module_)) {
            return {};
        }

        auto path = handleIdentifierLiteral(
                Context::enter(Context::LiteralExpr, context))
            .value_or(syntax::RequiredButMissing{});
        if (context.emitIfNodeEmpty(path, Reason::MissingId)) {
            return {};
        }

        context.emitIfLexemeNotPresent(
            lexeme::semicolon, Reason::MissingEnclosingSymbol
        );
        return syntax::global::ModuleDecl{
            std::move(path), context.location()
        };
    }


    auto handleImportDecl(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::import_)) {
            return {};
        }

        auto path_or_alias =
            handleIdentifierLiteral(Context::enter(Context::LiteralExpr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(path_or_alias, Reason::MissingId);

        syntax::Node path;
        if (context.stream().match(lexeme::equal)) {
            path = handleIdentifierLiteral(
                    Context::enter(Context::LiteralExpr, context))
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
