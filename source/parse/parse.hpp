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
#include "combinator.hpp"

namespace tlc::parse {
    class Parse final {
        using TError = Error<EParseErrorContext, EParseErrorReason>;
        using TErrorCollector =
        ErrorCollector<EParseErrorContext, EParseErrorReason>;
        using ParseResult = Expected<syntax::Node, TError>;
        using Reason = EParseErrorReason;

    public:
        static auto operator()(fs::path filepath, Vec<token::Token> tokens)
            -> syntax::Node;

        Parse(fs::path filepath, Vec<token::Token> tokens)
            : m_filepath{std::move(filepath)},
              m_stream{std::move(tokens)},
              m_tracker{m_stream}, m_collector{TErrorCollector::instance()},
              m_isSubroutine{false} {}

        auto operator()() -> syntax::Node;

#ifdef TLC_CONFIG_BUILD_TESTS
        auto parseType() -> ParseResult {
            return handleType();
        }

        auto parseExpr() -> ParseResult {
            return handleExpr();
        }

        auto parseStmt() -> ParseResult {
            return handleStmt();
        }

        auto parseDecl() -> ParseResult {
            return handleDecl();
        }

        auto parseGenericParamsDecl() -> ParseResult {
            return handleGenericParamsDecl();
        }
#endif

    private:
        Parse(fs::path filepath, Vec<token::Token> tokens, Location offset)
            : m_filepath{std::move(filepath)},
              m_stream{std::move(tokens), std::move(offset)},
              m_tracker{m_stream}, m_collector{TErrorCollector::instance()},
              m_isSubroutine{true} {}

    private:
        auto handleExpr(syntax::OpPrecedence minP = 0) -> ParseResult;
        auto handlePrimaryExpr() -> ParseResult;
        auto handleRecordExpr() -> ParseResult;
        auto handleTupleExpr() -> ParseResult;
        auto handleArrayExpr() -> ParseResult;
        auto handleSingleTokenLiteral() -> ParseResult;
        auto handleIdentifierLiteral() -> ParseResult;
        auto handleString() -> ParseResult;
        auto handleTryExpr() -> ParseResult;

        auto handleType(syntax::OpPrecedence minP = 0) -> ParseResult;
        auto handleTypeIdentifier() -> ParseResult;
        auto handleTypeTuple() -> ParseResult;
        auto handleTypeInfer() -> ParseResult;
        auto handleGenericArguments() -> ParseResult;

        auto handleDecl() -> ParseResult;
        auto handleIdentifierDecl() -> ParseResult;
        auto handleTupleDecl() -> ParseResult;
        auto handleGenericParamsDecl() -> ParseResult;

        auto handleStmt() -> ParseResult;
        auto handleDeclStmt() -> ParseResult;
        auto handleReturnStmt() -> ParseResult;
        auto handleDeferStmt() -> ParseResult;
        auto handleExprPrefixedStmt() -> ParseResult;
        auto handleLoopStmt() -> ParseResult;
        auto handleMatchStmt() -> ParseResult;
        auto handleBlockStmt() -> ParseResult;

        auto handleFunctionDef(token::Token const& visibility) -> ParseResult;
        auto handleFunctionPrototype() -> ParseResult;
        auto handleTypeDef(token::Token const& visibility) -> ParseResult;
        auto handleEnumDef(token::Token const& visibility) -> ParseResult;
        auto handleTraitDef(token::Token const& visibility) -> ParseResult;
        auto handleFlagDef(token::Token const& visibility) -> ParseResult;
        auto handleModuleDecl() -> ParseResult;
        auto handleImportDecl() -> ParseResult;
        auto handleTranslationUnit() -> ParseResult;

    private:
        static auto defaultError() -> ParseResult {
            return Unexpected{TError{}};
        }

        [[nodiscard]] auto error(TError::Params params) const
            -> Unexpected<TError> {
            params.filepath = m_filepath;
            params.location = m_tracker.current();
            return Unexpected<TError>{std::move(params)};
        }

        auto collect(TError::Params errorParams) const -> TErrorCollector& {
            // todo: remove this if
            if (errorParams.reason == EParseErrorReason::NotAnError) {
                return m_collector;
            }
            errorParams.filepath = m_filepath;
            errorParams.location = m_tracker.current();
            return m_collector(TError{std::move(errorParams)});
        }

        // todo: remove this
        auto collect(TError error) const -> TErrorCollector& {
            if (error.reason() == EParseErrorReason::NotAnError) {
                return m_collector;
            }

            error.filepath(m_filepath);
            return m_collector(std::move(error));
        }

        [[nodiscard]] auto createDefaultVisibility() const -> token::Token {
            return {lexeme::empty, "", m_stream.peek().location()};
        }

        [[nodiscard]] auto enter(EParseErrorContext const errorContext)
            -> Context {
            return Context{
                m_filepath, m_stream, m_tracker, m_collector, errorContext
            };
        }

    private:
        fs::path m_filepath;
        TokenStream m_stream;
        LocationTracker m_tracker;
        TErrorCollector& m_collector;
        b8 const m_isSubroutine;
    };
}

#endif // TLC_PARSE_HPP
