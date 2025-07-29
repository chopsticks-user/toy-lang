#include "util.hpp"
#include "nodes.hpp"

namespace tlc::syntax {
    auto isEmptyNode(Node const& node) -> bool {
        return std::holds_alternative<std::monostate>(node);
    }

    static const HashSet prefixOps = {
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

    static const HashSet postfixStart = {
        token::EToken::Dot,
        token::EToken::LeftParen,
        token::EToken::LeftBracket,
        // token::EToken::LeftBrace,
    };

    auto isPostfixStart(token::EToken const type) -> bool {
        return postfixStart.contains(type);
    }

    static const HashSet binaryOps = {
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Star,
        token::EToken::FwdSlash,
    };

    auto isBinaryOperator(token::EToken const type) -> bool {
        return binaryOps.contains(type);
    }

    static const HashMap<token::EToken, OpPrecedence>
    prefixOpPrecedenceTable = {
        {token::EToken::Exclaim, 30},
        {token::EToken::Tilde, 31},
        {token::EToken::Plus, 32},
        {token::EToken::Minus, 33},
        {token::EToken::Hash, 34},
        {token::EToken::Dot3, 35},
    };

    static const HashMap<token::EToken, OpPrecedence>
    binaryOpPrecedenceTable = {
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

    static const HashSet leftAssociativeOps = {
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Star,
        token::EToken::FwdSlash,
    };

    auto isLeftAssociative(token::EToken const type) -> b8 {
        return leftAssociativeOps.contains(type);
    }

    static const HashSet assignmentOps = {
        token::EToken::Equal,
        token::EToken::PlusEqual,
        token::EToken::MinusEqual,
        token::EToken::StarEqual,
        token::EToken::FwdSlashEqual,
        token::EToken::PercentEqual,
        token::EToken::Star2Equal,
        token::EToken::AmpersandEqual,
        token::EToken::BarEqual,
        token::EToken::HatEqual,
        token::EToken::Less2Equal,
        token::EToken::Greater2Equal,
    };

    auto isAssignmentOperator(token::EToken const type) -> b8 {
        return assignmentOps.contains(type);
    }
}
