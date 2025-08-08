#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Function: 0 params, 1 return",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "()->Void",

        .expectedAstPrint =
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 0\n"
        "├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Void')",

        .expectedPrettyPrint =
        "(() -> Void)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Function: 1 param, 1 return",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Int->Bool",

        .expectedAstPrint =
        "type::Function [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Identifier [@0:5] with (const, fund, path) = (true, true, 'Bool')",

        .expectedPrettyPrint =
        "(Int -> Bool)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Function: 2 param, 2 return",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "(Int,$foo.Bar)->(Int,Bool)",

        .expectedAstPrint =
        "type::Function [@0:0]\n"
        "├─ type::Tuple [@0:0] with size = 2\n"
        "   ├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:5] with (const, fund, path) = (false, false, 'foo.Bar')\n"
        "├─ type::Tuple [@0:16] with size = 2\n"
        "   ├─ type::Identifier [@0:17] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:21] with (const, fund, path) = (true, true, 'Bool')",

        .expectedPrettyPrint =
        "((Int, $foo.Bar) -> (Int, Bool))",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Function: Nested",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Int -> Float -> Bool -> foo.Bar",

        .expectedAstPrint =
        "type::Function [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Function [@0:7]\n"
        "   ├─ type::Identifier [@0:7] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Function [@0:16]\n"
        "      ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Bool')\n"
        "      ├─ type::Identifier [@0:24] with (const, fund, path) = (true, false, 'foo.Bar')",

        .expectedPrettyPrint =
        "(Int -> (Float -> (Bool -> foo.Bar)))",
    });
}
