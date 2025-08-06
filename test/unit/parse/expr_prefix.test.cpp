#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Prefix: Available operators",
    "[Unit][Parse][Expr]"
) {
    auto const op = GENERATE(from_range(
        prefixOpPrecedenceTable | tlc::rv::keys | tlc::rv::transform(
            [](auto const& lexeme) {
            return lexeme.str();
            }
        ) | tlc::rng::to<tlc::Vec<tlc::Str>>()
    ));
    assertExpr({
        .source = op + "foo",

        .expectedAstPrint = std::format(
            "expr::Prefix [@0:0] with op = '{}'\n"
            "├─ expr::Identifier [@0:{}] with path = 'foo'",
            op, op.length()
        ),

        .expectedPrettyPrint = op + "foo",
    });
}
