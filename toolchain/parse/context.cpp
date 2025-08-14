#include "context.hpp"

namespace tlc::parse {
    // static HashMap<EContext, HashMap<EContext, HashSet<lexeme::Lexeme>>> const
    // contextSensitiveFollowSet{
    //     {
    //         EContext::TupleDecl, {
    //             {EContext::IdDecl, {lexeme::comma, lexeme::rightParen}}
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

    // auto Context::synchronize(EContext const childContext) const -> void {
    //     if (auto const currentIt = contextSensitiveFollowSet.find(m_errorContext);
    //         currentIt != contextSensitiveFollowSet.end()) {
    //         if (auto const parentIt = currentIt->second.find(childContext);
    //             parentIt != currentIt->second.end()) {
    //             auto const& terminals = parentIt->second;
    //             while (!(m_tokenStream.done() ||
    //                 terminals.contains(m_tokenStream.peek().lexeme()))) {
    //                 // todo: save dropped tokens
    //                 m_tokenStream.advance();
    //             }
    //         }
    //     }
    // }
}
