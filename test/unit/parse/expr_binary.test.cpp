#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Binary: Available operators",
    "[Unit][Parse][Expr]"
) {
    auto const op = GENERATE(from_range(
        binaryOpPrecedenceTable | tlc::rv::keys | tlc::rv::transform(
            [](auto const& lexeme) {
            return lexeme.str();
            }
        ) | tlc::rng::to<tlc::Vec<tlc::Str>>()
    ));
    assertExpr({
        .source = std::format("x{}y", op),

        .expectedAstPrint = std::format(
            "expr::Binary [@0:0] with op = '{}'\n"
            "├─ expr::Identifier [@0:0] with path = 'x'\n"
            "├─ expr::Identifier [@0:{}] with path = 'y'",
            op, 1 + op.length()
        ),

        .expectedPrettyPrint = std::format("(x {} y)", op),
    });
}

// todo: boundary cases
