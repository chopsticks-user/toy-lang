#include "util.hpp"
#include "nodes.hpp"

#ifdef TLC_CONFIG_BUILD_TESTS
#define TLC_STATIC_IF_NOT_BUILD_TESTS
#else
#define TLC_STATIC_IF_NOT_BUILD_TESTS static
#endif // TLC_CONFIG_BUILD_TESTS

namespace tlc::syntax {
    auto isEmptyNode(Node const& node) -> bool {
        return std::holds_alternative<std::monostate>(node);
    }

    // todo: check C operator precedence
    TLC_STATIC_IF_NOT_BUILD_TESTS
    const HashMap<lexeme::Lexeme, OpPrecedence>
    prefixOpPrecedenceTable = {
        {lexeme::exclaim, 40},
        {lexeme::tilde, 41},
        {lexeme::plus, 42},
        {lexeme::minus, 43},
        {lexeme::hash, 44},
        {lexeme::dot3, 45},
        {lexeme::ampersand, 46},
    };

    TLC_STATIC_IF_NOT_BUILD_TESTS
    const HashMap<lexeme::Lexeme, OpPrecedence>
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
        {lexeme::dot2, 26},
        {lexeme::minus, 28},
        {lexeme::plus, 28},
        {lexeme::fwdSlash, 30},
        {lexeme::star, 30},
        {lexeme::star2, 32},
        {lexeme::barGreater, 34},
    };

    const HashSet<lexeme::Lexeme> binaryTypeOpTable = {
        lexeme::bar, lexeme::ampersand,
        lexeme::equal2, lexeme::exclaimEqual,
        lexeme::barGreater,
    };

    TLC_STATIC_IF_NOT_BUILD_TESTS
    const HashSet<lexeme::Lexeme> leftAssociativeOps = {
        lexeme::bar2, lexeme::ampersand2, lexeme::bar, lexeme::hat,
        lexeme::ampersand, lexeme::exclaimEqual, lexeme::equal2,
        lexeme::greaterEqual, lexeme::lessEqual, lexeme::greater,
        lexeme::less, lexeme::less2, lexeme::greater2, lexeme::plus,
        lexeme::minus, lexeme::star, lexeme::fwdSlash, lexeme::star2,
        lexeme::barGreater, lexeme::dot2,
    };

    TLC_STATIC_IF_NOT_BUILD_TESTS
    const HashSet<lexeme::Lexeme> assignmentOps = {
        lexeme::plusEqual, lexeme::minusEqual, lexeme::starEqual,
        lexeme::fwdSlashEqual, lexeme::percentEqual, lexeme::star2Equal,
        lexeme::ampersandEqual, lexeme::barEqual, lexeme::hatEqual,
        lexeme::less2Equal, lexeme::greater2Equal, lexeme::colonEqual,
    };

    auto isPrefixOperator(lexeme::Lexeme const& lexeme) -> bool {
        return prefixOpPrecedenceTable.contains(lexeme);
    }

    static const HashSet postfixStart = {
        lexeme::leftParen, lexeme::leftBracket,
    };

    auto isPostfixStart(lexeme::Lexeme const& lexeme) -> bool {
        return postfixStart.contains(lexeme);
    }

    auto isBinaryOperator(lexeme::Lexeme const& lexeme) -> bool {
        return binaryOpPrecedenceTable.contains(lexeme);
    }

    auto isBinaryTypeOperator(lexeme::Lexeme const& lexeme) -> bool {
        return binaryTypeOpTable.contains(lexeme);
    }

    auto opPrecedence(
        lexeme::Lexeme const& lexeme, EOperator const opType
    ) -> OpPrecedence {
        switch (opType) {
        case EOperator::Prefix: return prefixOpPrecedenceTable.at(lexeme);
        case EOperator::Binary: return binaryOpPrecedenceTable.at(lexeme);
        default: return 0;
        }
    }

    auto isLeftAssociative(lexeme::Lexeme const& lexeme) -> b8 {
        return leftAssociativeOps.contains(lexeme);
    }

    auto isAssignmentOperator(lexeme::Lexeme const& lexeme) -> b8 {
        return assignmentOps.contains(lexeme);
    }
}
