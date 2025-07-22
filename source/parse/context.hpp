#ifndef TLC_PARSE_CONTEXT_HPP
#define TLC_PARSE_CONTEXT_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"

namespace tlc::parse {
    enum class EState {
        Literal1, NamespaceId
    };

    class Context final {
        struct State {
            EState type;
            token::TokenizedBuffer tokens;
            Vec<syntax::Node> nodes;
        };

    public:
        auto push(EState const state) -> void {
            m_states.push({.type = state, .tokens = {}});
        }

        // todo: expected
        auto pop() -> void {
            if (m_states.empty()) {
                return;
            }
            auto const state = m_states.top();
            m_states.pop();

            if (m_states.empty()) {
                return;
            }
            m_states.top().nodes.emplace_back(createNode(state));
        }

        // todo: expected
        auto append(token::TokenizedBuffer token) -> void {
            if (m_states.empty()) {
                return;
            }
            m_states.top().tokens.append_range(std::move(token));
        }

    private:
        // todo: expected
        static auto createNode(State const& state) -> syntax::Node;

    private:
        Stack<State> m_states;
    };
}

#endif // TLC_PARSE_CONTEXT_HPP
