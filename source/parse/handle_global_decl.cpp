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
        return {};
    }
}
