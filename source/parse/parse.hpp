#ifndef TLC_PARSE_HPP
#define TLC_PARSE_HPP

#include "core/core.hpp"
#include "token/token.hpp"
#include "syntax/syntax.hpp"

#include "context.hpp"
#include "panic.hpp"
#include "ast_printer.hpp"
#include "pretty_printer.hpp"
#include "token_stream.hpp"
#include "combinator.hpp"

namespace tlc::parse {
    class Parse final {
    public:
        using TokenIt = Vec<token::Token>::const_iterator;
        using ParseResult = Expected<syntax::Node, Error>;

    public:
        static auto operator()(fs::path filepath, Vec<token::Token> tokens) -> syntax::Node;

        Parse(fs::path filepath, Vec<token::Token> tokens)
            : m_stream{std::move(tokens)}, m_panic{std::move(filepath)} {}

        auto operator()() -> syntax::Node;

    public: // for testing
        auto parseExpr() -> ParseResult {
            return handleExpr();
        }

        auto parseType() -> ParseResult {
            return handleType();
        }

        auto parseStmt() -> ParseResult {
            return handleStmt();
        }

        auto parseDecl() -> ParseResult {
            return handleStmtLevelDecl();
        }

    private:
        auto handleExpr(syntax::OpPrecedence minP = 0) -> ParseResult;
        auto handlePrimaryExpr() -> ParseResult;
        auto handleRecordExpr() -> ParseResult;
        auto handleTupleExpr() -> ParseResult;
        auto handleArrayExpr() -> ParseResult;
        auto handleSingleTokenLiteral() -> ParseResult;
        auto handleIdentifierLiteral() -> ParseResult;

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
        auto handleExprPrefixedStmt() -> ParseResult;
        auto handleLoopStmt() -> ParseResult;
        auto handleMatchStmt() -> ParseResult;
        auto handleConditionalStmt() -> ParseResult;
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
        auto pushCoords() -> void {
            // todo: eof
            return m_coords.push(m_stream.peek().coords());
        }

        auto popCoords() -> token::Token::Coords {
            auto coords = currentCoords();
            m_coords.pop();
            return coords;
        }

        auto currentCoords() -> token::Token::Coords {
            if (m_coords.empty()) {
                throw InternalError{
                    "Parser::popCoords: m_markedCoords.empty()"
                };
            }
            return m_coords.top();
        }

        static auto defaultError() -> ParseResult {
            return Unexpected{Error{}};
        }

    private:
        TokenStream m_stream;
        Context m_context{};
        Panic m_panic;
        Stack<token::Token::Coords> m_coords{};
    };
}

#endif // TLC_PARSE_HPP
