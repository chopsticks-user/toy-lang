export module parse:impl;

import token;
import syntax;
import :context;
import :error;
import :stream;
import :tracker;
import :unit_fwd;

namespace tlc::parse {
    export class Parse final {
        using TErrorCollector = ErrorCollector<EParseErrorContext, Reason>;

    public:
        static constexpr auto operator()(fs::path filepath, Vec<token::Token> tokens)
            -> syntax::Node {
            return Parse{std::move(filepath), std::move(tokens)}();
        }

        constexpr Parse(fs::path filepath, Vec<token::Token> tokens)
            : m_filepath{std::move(filepath)},
              m_stream{std::move(tokens)},
              m_tracker{m_stream}, m_collector{TErrorCollector::instance()},
              m_isSubroutine{false} {}

        constexpr Parse(fs::path filepath, Vec<token::Token> tokens, Location offset)
            : m_filepath{std::move(filepath)},
              m_stream{std::move(tokens), std::move(offset)},
              m_tracker{m_stream}, m_collector{TErrorCollector::instance()},
              m_isSubroutine{true} {}

        constexpr auto operator()() -> syntax::Node {
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

        constexpr auto globalContext(EParseErrorContext const eContext) -> Context {
            return Context::global(
                m_filepath, m_stream, m_tracker, m_collector,
                eContext, m_isSubroutine
            );
        }

        constexpr auto parseType() -> Opt<syntax::Node> {
            auto parent = globalContext(Context::Unknown);
            return handleType(Context::enter(Context::Type, parent));
        }

        constexpr auto parseExpr() -> Opt<syntax::Node> {
            auto parent = globalContext(Context::Unknown);
            return handleExpr(Context::enter(Context::Expr, parent));
        }

        constexpr auto parseStmt() -> Opt<syntax::Node> {
            auto parent = globalContext(Context::Unknown);
            return handleStmt(Context::enter(Context::Stmt, parent));
        }

        constexpr auto parseDecl() -> Opt<syntax::Node> {
            auto parent = globalContext(Context::Unknown);
            return handleDecl(Context::enter(Context::Decl, parent));
        }

        constexpr auto parseGenericParamsDecl() -> Opt<syntax::Node> {
            auto parent = globalContext(Context::Unknown);
            return handleGenericParamsDecl(
                Context::enter(Context::GenericParamsDecl, parent));
        }

    private:
        fs::path m_filepath;
        TokenStream m_stream;
        LocationTracker m_tracker;
        TErrorCollector& m_collector;
        b8 const m_isSubroutine;
    };
}
