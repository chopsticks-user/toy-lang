#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Binary: Available operators",
    "[Unit][Parse][Type]"
) {
    auto const op = GENERATE(from_range(
        binaryTypeOpTable | tlc::rv::transform(
            [](auto const& lexeme) {
            return lexeme.str();
            }
        ) | tlc::rng::to<tlc::Vec<tlc::Str>>()
    ));
    assertType({
        .source = std::format("Bar{}$Int", op),

        .expectedAstPrint = std::format(
            "type::Binary [@0:0] with op = '{}'\n"
            "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'Bar')\n"
            "├─ type::Identifier [@0:{}] with (const, fund, path) = (false, true, 'Int')",
            op, 3 + op.length()
        ),

        .expectedPrettyPrint = std::format("(Bar {} $Int)", op),
    });
}
