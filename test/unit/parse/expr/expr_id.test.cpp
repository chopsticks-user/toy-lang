#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Local",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "baz",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = 'baz'",

        .expectedPrettyPrint =
        "baz",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Imported",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo.bar",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = 'foo.bar'",

        .expectedPrettyPrint =
        "foo.bar",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Nested",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo.bar.baz.boo",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = 'foo.bar.baz.boo'",

        .expectedPrettyPrint =
        "foo.bar.baz.boo",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Anonymous",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "_",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = '_'",

        .expectedPrettyPrint =
        "_",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Longest valid identifier",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo.bar.baz_boo",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = 'foo.bar.baz'",

        .expectedPrettyPrint =
        "foo.bar.baz",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: Sub-identifiers of an anonymous identifier are ignored",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "_.foo.bar.baz",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = '_'",

        .expectedPrettyPrint =
        "_",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Identifier: snake_case is undefined and divided into multiple parts",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo_bar_baz",

        .expectedAstPrint =
        "expr::Identifier [@0:0] with path = 'foo'",

        .expectedPrettyPrint =
        "foo",
    });
}
