#ifndef TLC_PARSE_CONTEXT_HPP
#define TLC_PARSE_CONTEXT_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"

namespace tlc::parse {
    enum class EContext {};

    class Context final {
    public:
    private:
        Stack<token::Token> m_states{};
        Vec<syntax::Node> m_nodes{};
    };
}

#endif // TLC_PARSE_CONTEXT_HPP
