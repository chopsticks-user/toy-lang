#ifndef TLC_PARSE_HPP
#define TLC_PARSE_HPP

#include "context.hpp"
#include "panic.hpp"
#include "pretty_print.hpp"
#include "token_stream.hpp"
#include "combinators.hpp"

namespace tlc::parse {
    class Parser final {
        using TTokenIt = Vec<token::Token>::const_iterator;

    public:
        static auto operator()(Vec<token::Token> tokens) -> syntax::Node;

        explicit Parser(Vec<token::Token> tokens)
            : m_stream{std::move(tokens)} {}

        auto operator()() -> syntax::Node;

        [[nodiscard]] auto panic() -> syntax::Node {
            // todo:
            return {};
        }

        // for testing
    protected:
        // auto parseExpr() -> syntax::Node;
        // auto parseStmt() -> syntax::Node;

    private:
        auto parseModule() -> syntax::Node;

    private:
        TokenStream m_stream;
        Context m_context{};
    };
}

#endif // TLC_PARSE_HPP
