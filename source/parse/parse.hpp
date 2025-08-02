#ifndef TLC_PARSE_HPP
#define TLC_PARSE_HPP

#include "core/core.hpp"
#include "token/token.hpp"
#include "syntax/syntax.hpp"

#include "parse_error.hpp"
#include "ast_printer.hpp"
#include "pretty_printer.hpp"
#include "token_stream.hpp"
#include "combinator.hpp"
#include "location_tracker.hpp"

namespace tlc::parse {
    class Parse final {
        using TokenIt = Vec<token::Token>::const_iterator;
        using TError = Error<EParseErrorContext, EParseErrorReason>;
        using TErrorCollector =
        ErrorCollector<EParseErrorContext, EParseErrorReason>;
        using ParseResult = Expected<syntax::Node, TError>;

    public:
        static auto operator()(fs::path filepath, Vec<token::Token> tokens) -> syntax::Node;

        Parse(fs::path filepath, Vec<token::Token> tokens, Opt<Location> offset = {})
            : m_filepath{std::move(filepath)},
              m_stream{std::move(tokens), std::move(offset)},
              m_tracker{m_stream}, m_isSubroutine{offset} {}

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
            return handleStmtLevelDecl();
        }
#endif

    private:
        auto handleExpr(syntax::OpPrecedence minP = 0) -> ParseResult;
        auto handlePrimaryExpr() -> ParseResult;
        auto handleRecordExpr() -> ParseResult;
        auto handleTupleExpr() -> ParseResult;
        auto handleArrayExpr() -> ParseResult;
        auto handleSingleTokenLiteral() -> ParseResult;
        auto handleIdentifierLiteral() -> ParseResult;
        auto handleString() -> ParseResult;

        auto handleType() -> ParseResult;
        auto handleTypeIdentifier() -> ParseResult;
        auto handleTypeTuple() -> ParseResult;
        auto handleTypeInfer() -> ParseResult;

        auto handleStmtLevelDecl() -> ParseResult;
        auto handleIdentifierDecl() -> ParseResult;
        auto handleTupleDecl() -> ParseResult;

        auto handleStmt() -> ParseResult;
        auto handleLetStmt() -> ParseResult;
        auto handleReturnStmt() -> ParseResult;
        auto handleDeferStmt() -> ParseResult;
        auto handlePrefaceStmt() -> ParseResult;
        auto handleYieldStmt() -> ParseResult;
        auto handleExprPrefixedStmt() -> ParseResult;
        auto handleLoopStmt() -> ParseResult;
        auto handleMatchStmt() -> ParseResult;
        auto handleBlockStmt() -> ParseResult;

        auto handleFunctionDef() -> ParseResult;
        auto handleFunctionPrototype() -> ParseResult;
        auto handleTypeDef() -> ParseResult;
        auto handleEnumDef() -> ParseResult;
        auto handleTraitDef() -> ParseResult;
        auto handleFlagDef() -> ParseResult;
        auto handleModuleDecl() -> ParseResult;
        auto handleImportDecl() -> ParseResult;
        auto handleTranslationUnit() -> ParseResult;

    private:
        // todo: move a to separate class
        auto pushCoords() -> void {
            // todo: eof
            return m_coords.push(m_stream.peek().location());
        }

        auto popCoords() -> Location {
            auto const coords = currentCoords();
            m_coords.pop();
            return coords;
        }

        auto currentCoords() -> Location {
            if (m_coords.empty()) {
                throw InternalException{
                    "Parser::popCoords: m_markedCoords.empty()"
                };
            }
            return m_coords.top();
        }

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
            errorParams.filepath = m_filepath;
            return TErrorCollector::instance().collect(std::move(errorParams));
        }

        auto collect(TError error) const -> TErrorCollector& {
            error.filepath(m_filepath);
            return TErrorCollector::instance().collect(std::move(error));
        }

    private:
        fs::path m_filepath;
        TokenStream m_stream;
        LocationTracker m_tracker;
        Stack<Location> m_coords{};
        b8 const m_isSubroutine;
    };
}

#endif // TLC_PARSE_HPP
