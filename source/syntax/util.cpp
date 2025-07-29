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
        token::EToken::Star2,
        token::EToken::Star,
        token::EToken::FwdSlash,
        token::EToken::Plus,
        token::EToken::Minus,
        token::EToken::Greater2,
        token::EToken::Less2,
        token::EToken::Less,
        token::EToken::Greater,
        token::EToken::LessEqual,
        token::EToken::GreaterEqual,
        token::EToken::Equal2,
        token::EToken::ExclaimEqual,
        token::EToken::Ampersand,
        token::EToken::Hat,
        token::EToken::Bar,
        token::EToken::Ampersand2,
        token::EToken::Bar2,
    };

    auto isBinaryOperator(token::EToken const type) -> bool {
        return binaryOps.contains(type);
    }

    static const HashMap<token::EToken, OpPrecedence>
    prefixOpPrecedenceTable = {
        {token::EToken::Exclaim, 40},
        {token::EToken::Tilde, 41},
        {token::EToken::Plus, 42},
        {token::EToken::Minus, 43},
        {token::EToken::Hash, 44},
        {token::EToken::Dot3, 45},
    };

    static const HashMap<token::EToken, OpPrecedence>
    binaryOpPrecedenceTable = {
        {token::EToken::Bar2, 10},
        {token::EToken::Ampersand2, 12},
        {token::EToken::Bar, 14},
        {token::EToken::Hat, 16},
        {token::EToken::Ampersand, 18},
        {token::EToken::ExclaimEqual, 20},
        {token::EToken::Equal2, 20},
        {token::EToken::GreaterEqual, 22},
        {token::EToken::LessEqual, 22},
        {token::EToken::Greater, 22},
        {token::EToken::Less, 22},
        {token::EToken::Less2, 24},
        {token::EToken::Greater2, 24},
        {token::EToken::Minus, 26},
        {token::EToken::Plus, 26},
        {token::EToken::FwdSlash, 28},
        {token::EToken::Star, 28},
        {token::EToken::Star2, 30},
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
