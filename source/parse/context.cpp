#include "context.hpp"

namespace tlc::parse {
    using EContext = EParseErrorContext;

    // HashMap<EContext, HashMap<EContext, HashSet<lexeme::Lexeme>>> const
    // contextSensitiveFollowSet{
    //     {
    //         EContext::IdDecl, {
    //             {EContext::TupleDecl, {lexeme::comma, lexeme::rightParen}}
    //         }
    //     },
    // };

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
        // switch (m_errorContext) {
        // case IdDecl: {
        //     static HashMap<EContext, HashSet<lexeme::Lexeme> const follow
        // }
        // }
        // if (auto const currentIt = contextSensitiveFollowSet.find(m_errorContext);
        //     currentIt != contextSensitiveFollowSet.end()) {}
    }
}
