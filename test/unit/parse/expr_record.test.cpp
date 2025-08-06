#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Record: Empty",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "{}",

        .expectedAstPrint =
        "expr::Record [@0:0] with size = 0\n"
        "├─ (empty)",

        .expectedPrettyPrint =
        "{?}{}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Record: Type present, 0 entries",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "foo.Bar{}",

        .expectedAstPrint =
        "expr::Record [@0:0] with size = 0\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')",

        .expectedPrettyPrint =
        "foo.Bar{}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Record: Type present, 3 entries",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "foo.Bar{x:5,y:3.14,z:true}",

        .expectedAstPrint =
        "expr::Record [@0:0] with size = 3\n"
        "├─ type::Identifier [@0:0] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "├─ expr::RecordEntry [@0:8] with key = 'x'\n"
        "   ├─ expr::Integer [@0:10] with value = 5\n"
        "├─ expr::RecordEntry [@0:12] with key = 'y'\n"
        "   ├─ expr::Float [@0:14] with value = 3.14\n"
        "├─ expr::RecordEntry [@0:19] with key = 'z'\n"
        "   ├─ expr::Boolean [@0:21] with value = true",

        .expectedPrettyPrint =
        "foo.Bar{x: 5, y: 3.14, z: true}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Record: Nested",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "{x:5,y:{},z:{zx: foo.Baz{x: foo.bar, zxy: 0}} }",

        .expectedAstPrint =
        "expr::Record [@0:0] with size = 3\n"
        "├─ (empty)\n"
        "├─ expr::RecordEntry [@0:1] with key = 'x'\n"
        "   ├─ expr::Integer [@0:3] with value = 5\n"
        "├─ expr::RecordEntry [@0:5] with key = 'y'\n"
        "   ├─ expr::Record [@0:7] with size = 0\n"
        "      ├─ (empty)\n"
        "├─ expr::RecordEntry [@0:10] with key = 'z'\n"
        "   ├─ expr::Record [@0:12] with size = 1\n"
        "      ├─ (empty)\n"
        "      ├─ expr::RecordEntry [@0:13] with key = 'zx'\n"
        "         ├─ expr::Record [@0:17] with size = 2\n"
        "            ├─ type::Identifier [@0:17] with (const, fund, path) = (true, false, 'foo.Baz')\n"
        "            ├─ expr::RecordEntry [@0:25] with key = 'x'\n"
        "               ├─ expr::Identifier [@0:28] with path = 'foo.bar'\n"
        "            ├─ expr::RecordEntry [@0:37] with key = 'zxy'\n"
        "               ├─ expr::Integer [@0:42] with value = 0",

        .expectedPrettyPrint =
        "{?}{x: 5, y: {?}{}, z: {?}{zx: foo.Baz{x: foo.bar, zxy: 0}}}",
    });
}

// todo: error cases
