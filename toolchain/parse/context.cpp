#include "context.hpp"

namespace tlc::parse {
    using EContext = EContext;

    static HashMap<EContext, HashMap<EContext, HashSet<lexeme::Lexeme>>> const
    contextSensitiveFollowSet{
        {
            EContext::IdDecl, {
                {EContext::TupleDecl, {lexeme::comma, lexeme::rightParen}}
            }
        },
    };

    // HashMap<EContext, HashSet<lexeme::Lexeme>> const
    // contextSensitiveStartSet{
    //     {
    //         EContext::IdDecl, {
    //             lexeme::identifier, lexeme::static_, lexeme::isolated
    //         }
    //     },
    //     {
    //         EContext::TupleDecl, {
    //             lexeme::leftParen
    //         }
    //     },
    //     {
    //         EContext::GenericParamsDecl, {
    //             lexeme::less
    //         }
    //     },
    // };

    auto Context::synchronize() -> void {
        // if (auto const currentIt = contextSensitiveFollowSet.find(m_errorContext);
        //     currentIt != contextSensitiveFollowSet.end()) {
        //     if (auto const parentIt = currentIt->second.find(m_errorContext);
        //         parentIt != currentIt->second.end()) {
        //         while (m_tokenStream.done())
        //         return;
        //     }
        // }
    }
}
