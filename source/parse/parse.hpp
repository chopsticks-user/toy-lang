#ifndef TLC_PARSE_HPP
#define TLC_PARSE_HPP

#include "core/core.hpp"
#include "token/token.hpp"
#include "syntax/syntax.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "pretty_print.hpp"
#include "token_stream.hpp"
#include "combinator.hpp"

namespace tlc::parse {
    class Parser {
    public:
        using TokenIt = Vec<token::Token>::const_iterator;
        using ParseResult = Expected<syntax::Node, Error>;

    public:
        static auto operator()(Vec<token::Token> tokens) -> syntax::Node;

        explicit Parser(Vec<token::Token> tokens)
            : m_stream{std::move(tokens)} {}

        auto operator()() -> syntax::Node;

        // for testing
        auto parseExpr() -> ParseResult {
            return handleExpr();
        }

    protected:
        auto handleExpr(syntax::OpPrecedence minP = 0) -> ParseResult;
        auto handlePostfixExpr() -> ParseResult;
        auto handlePrimaryExpr() -> ParseResult;
        auto handleTupleExpr() -> ParseResult;
        auto handleArrayExpr() -> ParseResult;
        auto handleSingleTokenLiteral() -> ParseResult;
        auto handleIdentifierLiteral() -> ParseResult;

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

        template <typename... Args, syntax::IsASTNode T>
        auto createNode(token::Token::Coords coords, Args&&... args) -> syntax::Node {
            return T{std::forward<Args&&>(args)..., std::move(coords)};
        }

    private:
        TokenStream m_stream;
        Context m_context{};
        Panic m_panic{};
        Stack<token::Token::Coords> m_coords{};
    };
}

#endif // TLC_PARSE_HPP
