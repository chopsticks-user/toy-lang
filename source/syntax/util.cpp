#include "util.hpp"
#include "nodes.hpp"

namespace tlc::syntax {
    auto isEmptyNode(const Node& node) -> bool {
        return std::holds_alternative<std::monostate>(node);
    }

    static HashSet prefixOps = {
        token::EToken::Exclaim,
        token::EToken::Tilde,
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Hash,
        token::EToken::Dot3,
    };

    auto isPrefixOperator(token::EToken const type) -> bool {
        return prefixOps.contains(type);
    }

    static HashSet binaryOps = {
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Star,
        token::EToken::FwdSlash,
    };

    auto isBinaryOperator(token::EToken const type) -> bool {
        return binaryOps.contains(type);
    }

    static HashMap<token::EToken, OpPrecedence> prefixOpPrecedenceTable = {
        {token::EToken::Exclaim, 30},
        {token::EToken::Tilde, 31},
        {token::EToken::Plus, 32},
        {token::EToken::Minus, 33},
        {token::EToken::Hash, 34},
        {token::EToken::Dot3, 35},
    };

    static HashMap<token::EToken, OpPrecedence> binaryOpPrecedenceTable = {
        {token::EToken::Plus, 10},
        {token::EToken::Minus, 10},
        {token::EToken::Star, 12},
        {token::EToken::FwdSlash, 12},
    };

    auto opPrecedence(
        token::EToken const tokenType, EOperator const opType
    ) -> OpPrecedence {
        switch (opType) {
        case EOperator::Prefix: return prefixOpPrecedenceTable.at(tokenType);
        case EOperator::Binary: return binaryOpPrecedenceTable.at(tokenType);
        default: return 0;
        }
    }

    static HashSet leftAssociativeOps = {
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Star,
        token::EToken::FwdSlash,
    };

    auto isLeftAssociative(token::EToken const type) -> bool {
        return leftAssociativeOps.contains(type);
    }
}
