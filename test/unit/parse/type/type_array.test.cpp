#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Array: Empty",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "$foo.Bar[]",

        .expectedAstPrint =
        "type::Array [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (false, false, 'foo.Bar')\n"
        "├─ expr::Array [@0:8] with size = 0",

        .expectedPrettyPrint =
        "$foo.Bar[]",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Array: 1 dimension",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "[[x]][5,y+z]",

        .expectedAstPrint =
        "type::Array [@0:0]\n"
        "├─ type::Infer [@0:0]\n"
        "   ├─ expr::Identifier [@0:2] with path = 'x'\n"
        "├─ expr::Array [@0:5] with size = 2\n"
        "   ├─ expr::Integer [@0:6] with value = 5\n"
        "   ├─ expr::Binary [@0:8] with op = '+'\n"
        "      ├─ expr::Identifier [@0:8] with path = 'y'\n"
        "      ├─ expr::Identifier [@0:10] with path = 'z'",

        .expectedPrettyPrint =
        "[[ x ]][5, (y + z)]",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Array: 4 dimensions",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Int[3,x+1,1,4]",

        .expectedAstPrint =
        "type::Array [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ expr::Array [@0:3] with size = 4\n"
        "   ├─ expr::Integer [@0:4] with value = 3\n"
        "   ├─ expr::Binary [@0:6] with op = '+'\n"
        "      ├─ expr::Identifier [@0:6] with path = 'x'\n"
        "      ├─ expr::Integer [@0:8] with value = 1\n"
        "   ├─ expr::Integer [@0:10] with value = 1\n"
        "   ├─ expr::Integer [@0:12] with value = 4",

        .expectedPrettyPrint =
        "Int[3, (x + 1), 1, 4]",
    });
}
