#include "util.hpp"
#include "nodes.hpp"

namespace tlc::syntax {
    auto isEmptyNode(Node const& node) -> bool {
        return std::holds_alternative<std::monostate>(node);
    }

    static const HashSet prefixOps = {
        lexeme::exclaim, lexeme::tilde, lexeme::plus,
        lexeme::minus, lexeme::hash, lexeme::dot3,
        lexeme::ampersand,
    };

    auto isPrefixOperator(lexeme::Lexeme const& lexeme) -> bool {
        return prefixOps.contains(lexeme);
    }

    static const HashSet postfixStart = {
        lexeme::dot, lexeme::leftParen, lexeme::leftBracket,
    };

    auto isPostfixStart(lexeme::Lexeme const& lexeme) -> bool {
        return postfixStart.contains(lexeme);
    }

    static const HashSet binaryOps = {
        lexeme::star2, lexeme::star, lexeme::fwdSlash, lexeme::plus,
        lexeme::minus, lexeme::greater2, lexeme::less2, lexeme::less,
        lexeme::greater, lexeme::lessEqual, lexeme::greaterEqual,
        lexeme::equal2, lexeme::exclaimEqual, lexeme::ampersand,
        lexeme::hat, lexeme::bar, lexeme::ampersand2, lexeme::bar2,
        lexeme::barGreater,
    };

    auto isBinaryOperator(lexeme::Lexeme const& lexeme) -> bool {
        return binaryOps.contains(lexeme);
    }

    // todo: check C operator precedence
    static const HashMap<lexeme::Lexeme, OpPrecedence>
    prefixOpPrecedenceTable = {
        {lexeme::exclaim, 40},
        {lexeme::tilde, 41},
        {lexeme::plus, 42},
        {lexeme::minus, 43},
        {lexeme::hash, 44},
        {lexeme::dot3, 45},
        {lexeme::ampersand, 46},
    };

    static const HashMap<lexeme::Lexeme, OpPrecedence>
    binaryOpPrecedenceTable = {
        {lexeme::bar2, 10},
        {lexeme::ampersand2, 12},
        {lexeme::bar, 14},
        {lexeme::hat, 16},
        {lexeme::ampersand, 18},
        {lexeme::exclaimEqual, 20},
        {lexeme::equal2, 20},
        {lexeme::greaterEqual, 22},
        {lexeme::lessEqual, 22},
        {lexeme::greater, 22},
        {lexeme::less, 22},
        {lexeme::less2, 24},
        {lexeme::greater2, 24},
        {lexeme::minus, 26},
        {lexeme::plus, 26},
        {lexeme::fwdSlash, 28},
        {lexeme::star, 28},
        {lexeme::star2, 30},
        {lexeme::barGreater, 32},
    };

    auto opPrecedence(
        lexeme::Lexeme const& lexeme, EOperator const opType
    ) -> OpPrecedence {
        switch (opType) {
        case EOperator::Prefix: return prefixOpPrecedenceTable.at(lexeme);
        case EOperator::Binary: return binaryOpPrecedenceTable.at(lexeme);
        default: return 0;
        }
    }

    static const HashSet leftAssociativeOps = {
        lexeme::bar2, lexeme::ampersand2, lexeme::bar, lexeme::hat,
        lexeme::ampersand, lexeme::exclaimEqual, lexeme::equal2,
        lexeme::greaterEqual, lexeme::lessEqual, lexeme::greater,
        lexeme::less, lexeme::less2, lexeme::greater2, lexeme::plus,
        lexeme::minus, lexeme::star, lexeme::fwdSlash, lexeme::star2,
        lexeme::barGreater,
    };

    auto isLeftAssociative(lexeme::Lexeme const& lexeme) -> b8 {
        return leftAssociativeOps.contains(lexeme);
    }

    static const HashSet assignmentOps = {
        lexeme::equal, lexeme::plusEqual, lexeme::minusEqual, lexeme::starEqual,
        lexeme::fwdSlashEqual, lexeme::percentEqual, lexeme::star2Equal,
        lexeme::ampersandEqual, lexeme::barEqual, lexeme::hatEqual,
        lexeme::less2Equal, lexeme::greater2Equal,
    };

    auto isAssignmentOperator(lexeme::Lexeme const& lexeme) -> b8 {
        return assignmentOps.contains(lexeme);
    }
}
