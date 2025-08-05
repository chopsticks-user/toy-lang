#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Literal: Integer",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "31415",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 31415",

        .expectedPrettyPrint =
        "31415",
    });
    assertExprWithParams({
        .source =
        "0",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
    assertExprWithParams({
        .source =
        "0x123456789abcdef",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 81985529216486895",

        .expectedPrettyPrint =
        "81985529216486895",
    });
    assertExprWithParams({
        .source =
        "0x0",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
    assertExprWithParams({
        .source =
        "01234567",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 342391",

        .expectedPrettyPrint =
        "342391",
    });
    assertExprWithParams({
        .source =
        "00",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
    assertExprWithParams({
        .source =
        "0b1010101001",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 681",

        .expectedPrettyPrint =
        "681",
    });
    assertExprWithParams({
        .source =
        "0b0",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Literal: Float",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "0.0314159",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0.0314159",

        .expectedPrettyPrint =
        "0.0314159",
    });
    assertExprWithParams({
        .source =
        "3145.1926",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 3145.1926",

        .expectedPrettyPrint =
        "3145.1926",
    });
    assertExprWithParams({
        .source =
        "0.314159",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0.314159",

        .expectedPrettyPrint =
        "0.314159",
    });
    assertExprWithParams({
        .source =
        "00.31415",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0.31415",

        .expectedPrettyPrint =
        "0.31415",
    });
    assertExprWithParams({
        .source =
        "000.3141",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0.3141",

        .expectedPrettyPrint =
        "0.3141",
    });
    assertExprWithParams({
        .source =
        "0.000",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
    assertExprWithParams({
        .source =
        "00.00",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
    assertExprWithParams({
        .source =
        "000.0",

        .expectedAstPrint =
        "expr::Float [@0:0] with value = 0",

        .expectedPrettyPrint =
        "0",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Literal: Boolean",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "true",

        .expectedAstPrint =
        "expr::Boolean [@0:0] with value = true",

        .expectedPrettyPrint =
        "true",
    });
    assertExprWithParams({
        .source =
        "false",

        .expectedAstPrint =
        "expr::Boolean [@0:0] with value = false",

        .expectedPrettyPrint =
        "false",
    });
}
