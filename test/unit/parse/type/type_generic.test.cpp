#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Generic: 1 argument",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Own<foo.Bar>",

        .expectedAstPrint =
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, true, 'Own')\n"
        "├─ type::GenericArguments [@0:3] with size = 1\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, false, 'foo.Bar')",

        .expectedPrettyPrint =
        "Own<foo.Bar>",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Generic: 3 arguments",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "foo.Bar<Int, Float, Bool>",

        .expectedAstPrint =
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ type::GenericArguments [@0:7] with size = 3\n"
        "   ├─ type::Identifier [@0:8] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ type::Identifier [@0:13] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Identifier [@0:20] with (const, fund, path) = (true, true, 'Bool')",

        .expectedPrettyPrint =
        "foo.Bar<Int, Float, Bool>",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Generic: Nested",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "foo.Bar<foo.Baz<Int, Float>, Bool>",

        .expectedAstPrint =
        "type::Generic [@0:0]\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ type::GenericArguments [@0:7] with size = 2\n"
        "   ├─ type::Generic [@0:8]\n"
        "      ├─ type::Identifier [@0:8] with (const, fund, path) = (true, false, 'foo.Baz')\n"
        "      ├─ type::GenericArguments [@0:15] with size = 2\n"
        "         ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Int')\n"
        "         ├─ type::Identifier [@0:21] with (const, fund, path) = (true, true, 'Float')\n"
        "   ├─ type::Identifier [@0:29] with (const, fund, path) = (true, true, 'Bool')",

        .expectedPrettyPrint =
        "foo.Bar<foo.Baz<Int, Float>, Bool>",
    });
}
