#ifndef TLC_PARSE_HPP
#define TLC_PARSE_HPP

#include "core/core.hpp"
#include "token/token.hpp"
#include "syntax/syntax.hpp"

#include "parse_error.hpp"
#include "token_stream.hpp"
#include "location_tracker.hpp"
#include "context.hpp"
#include "ast_printer.hpp"
#include "pretty_printer.hpp"

namespace tlc::parse {
    class Parse final {
        using TErrorCollector = ErrorCollector<EParseErrorContext, Reason>;

    public:
        static auto operator()(fs::path filepath, Vec<token::Token> tokens)
            -> syntax::Node;

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

        auto operator()() -> syntax::Node;

        constexpr auto globalContext(EParseErrorContext const eContext) -> Context {
            return Context::global(
                m_filepath, m_stream, m_tracker, m_collector,
                eContext, m_isSubroutine
            );
        }

#ifdef TLC_CONFIG_BUILD_TESTS
        auto parseType() -> Opt<syntax::Node>;
        auto parseExpr() -> Opt<syntax::Node>;
        auto parseStmt() -> Opt<syntax::Node>;
        auto parseDecl() -> Opt<syntax::Node>;
        auto parseGenericParamsDecl() -> Opt<syntax::Node>;
#endif

    private:
        fs::path m_filepath;
        TokenStream m_stream;
        LocationTracker m_tracker;
        TErrorCollector& m_collector;
        b8 const m_isSubroutine;
    };
}

#endif // TLC_PARSE_HPP
