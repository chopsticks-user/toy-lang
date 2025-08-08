#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Tuple: Empty",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "()",

        .expectedAstPrint =
        "type::Tuple [@0:0] with size = 0",

        .expectedPrettyPrint =
        "()",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Tuple: 1 element",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "($foo.Bar)",

        .expectedAstPrint =
        "type::Tuple [@0:0] with size = 1\n"
        "├─ type::Identifier [@0:1] with (const, fund, path) = (false, false, 'foo.Bar')",

        .expectedPrettyPrint =
        "($foo.Bar)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Tuple: 3 element",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "(Int,$foo.Bar,Type)",

        .expectedAstPrint =
        "type::Tuple [@0:0] with size = 3\n"
        "├─ type::Identifier [@0:1] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ type::Identifier [@0:5] with (const, fund, path) = (false, false, 'foo.Bar')\n"
        "├─ type::Identifier [@0:14] with (const, fund, path) = (true, false, 'Type')",

        .expectedPrettyPrint =
        "(Int, $foo.Bar, Type)",
    });
}
