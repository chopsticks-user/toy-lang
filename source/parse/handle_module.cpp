#include "parse.hpp"

namespace tlc::parse {
    auto Parser::parseModule() -> syntax::Node {
        if (!m_stream.match(token::EToken::Module)) {
            return {};
        }

        m_stream.markCurrentToken();
        // if (m_stream.match(token::EToken::Identifier)) {
        //     return syntax::decl::Module{syntax::, m_stream.markedLine(), m_stream.markedColumn()};
        // }

        return panic();
    }
}
