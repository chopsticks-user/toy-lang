#include "parse.hpp"
#include "parse_unit_fwd.hpp"

namespace tlc::parse {
    auto Parse::operator()(fs::path filepath, Vec<token::Token> tokens) -> syntax::Node {
        return Parse{std::move(filepath), std::move(tokens)}();
    }

    auto Parse::operator()() -> syntax::Node {
        return handleTranslationUnit(Context::Params{
                .filepath = m_filepath,
                .tokenStream = m_stream,
                .locationTracker = m_tracker,
                .errorCollector = m_collector,
                .errorContext = Context::TranslationUnit,
            })
            .value_or(syntax::TranslationUnit{
                m_filepath, syntax::RequiredButMissing{},
                {}, {}
            });
    }

#ifdef TLC_CONFIG_BUILD_TESTS
    auto Parse::parseType() -> Opt<syntax::Node> {
        auto parent = globalContext(Context::Unknown);
        return handleType(Context::enter(Context::Type, parent));
    }

    auto Parse::parseExpr() -> Opt<syntax::Node> {
        auto parent = globalContext(Context::Unknown);
        return handleExpr(Context::enter(Context::Expr, parent));
    }

    auto Parse::parseStmt() -> Opt<syntax::Node> {
        auto parent = globalContext(Context::Unknown);
        return handleStmt(Context::enter(Context::Stmt, parent));
    }

    auto Parse::parseDecl() -> Opt<syntax::Node> {
        auto parent = globalContext(Context::Unknown);
        return handleDecl(Context::enter(Context::Decl, parent));
    }

    auto Parse::parseGenericParamsDecl() -> Opt<syntax::Node> {
        auto parent = globalContext(Context::Unknown);
        return handleGenericParamsDecl(
            Context::enter(Context::GenericParamsDecl, parent));
    }
#endif
}
