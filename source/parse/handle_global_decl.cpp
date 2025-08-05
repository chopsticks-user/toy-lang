#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleTranslationUnit() -> ParseResult {
        Vec<syntax::Node> definitions;

        if (auto moduleDecl = handleModuleDecl(); !moduleDecl) {
            return error({
                .context = EParseErrorContext::TranslationUnit,
                .reason = EParseErrorReason::MissingDecl,
            });
        }
        else { definitions.push_back(std::move(*moduleDecl)); }

        while (m_stream.peek().lexeme() != lexeme::invalid) {
            auto importDecl = handleImportDecl();
            if (!importDecl) {
                break;
            }
            definitions.push_back(std::move(*importDecl));
        }

        while (m_stream.peek().lexeme() != lexeme::invalid) {
            auto const visibility =
                m_stream.match(lexeme::pub, lexeme::prv)
                    ? m_stream.current()
                    : createDefaultVisibility();

            if (auto fnDef = handleFunctionDef(visibility); fnDef) {
                definitions.push_back(std::move(*fnDef));
            }
        }

        return syntax::TranslationUnit{m_filepath, std::move(definitions)};
    }

    auto Parse::handleModuleDecl() -> ParseResult {
        if (!m_stream.match(lexeme::module_)) {
            return defaultError();
        }

        auto location = m_tracker.current();
        auto path = handleIdentifierLiteral();
        if (!path) {
            collect(path.error());
            return error({
                .location = m_tracker.current(),
                .context = EParseErrorContext::ModuleDecl,
                .reason = EParseErrorReason::MissingId,
            });
        }

        if (!m_stream.match(lexeme::semicolon)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::ModuleDecl,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }
        return syntax::global::ModuleDecl{std::move(*path), std::move(location)};
    }


    auto Parse::handleImportDecl() -> ParseResult {
        if (!m_stream.match(lexeme::import_)) {
            return defaultError();
        }

        auto location = m_tracker.current();
        auto path_or_alias = *handleIdentifierLiteral().or_else(
            [this](auto&& err) -> ParseResult {
                collect(err).collect({
                    .location = m_tracker.current(),
                    .context = EParseErrorContext::ImportDecl,
                    .reason = EParseErrorReason::MissingId,
                });
                return {};
            }
        );

        syntax::Node path;
        if (m_stream.match(lexeme::equal)) {
            path = *handleIdentifierLiteral().or_else(
                [this](auto&& err) -> ParseResult {
                    collect(err).collect({
                        .location = m_tracker.current(),
                        .context = EParseErrorContext::ImportDecl,
                        .reason = EParseErrorReason::MissingExpr,
                    });
                    return {};
                }
            );
        }

        if (!m_stream.match(lexeme::semicolon)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::ImportDecl,
                .reason = EParseErrorReason::MissingEnclosingSymbol,
            });
        }
        return syntax::global::ImportDecl{
            std::move(path_or_alias), std::move(path), std::move(location)
        };
    }
}
