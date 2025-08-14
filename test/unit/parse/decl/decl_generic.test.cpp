#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Generic: Empty",
    "[Unit][Parse][Decl]"
) {
    assertGenericDecl({
        .source =
        "<>",

        .expectedAstPrint =
        "decl::GenericParameters [@0:0] with size = 0",

        .expectedPrettyPrint =
        "<>",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Generic: 3 params",
    "[Unit][Parse][Decl]"
) {
    assertGenericDecl({
        .source =
        "<T,U,V>",

        .expectedAstPrint =
        "decl::GenericParameters [@0:0] with size = 3\n"
        "├─ decl::GenericIdentifier [@0:1] with name = 'T'\n"
        "├─ decl::GenericIdentifier [@0:3] with name = 'U'\n"
        "├─ decl::GenericIdentifier [@0:5] with name = 'V'",

        .expectedPrettyPrint =
        "<T, U, V>",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Generic: Synchronize until next ','",
    "[Unit][Parse][Decl]"
) {
    assertGenericDecl({
        .source =
        "<t,u,V>",

        .expectedAstPrint =
        "decl::GenericParameters [@0:0] with size = 3\n"
        "├─ (required)\n"
        "├─ (required)\n"
        "├─ decl::GenericIdentifier [@0:5] with name = 'V'",

        .expectedPrettyPrint =
        "<{!}, {!}, V>",

        .expectedErrors = {
            {.context = Context::GenericParamsDecl, .reason = Reason::MissingTypeId},
            {.context = Context::GenericParamsDecl, .reason = Reason::MissingTypeId},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Generic: Synchronize until next '>'",
    "[Unit][Parse][Decl]"
) {
    assertGenericDecl({
        .source =
        "<u>",

        .expectedAstPrint =
        "decl::GenericParameters [@0:0] with size = 1\n"
        "├─ (required)",

        .expectedPrettyPrint =
        "<{!}>",

        .expectedErrors = {
            {.context = Context::GenericParamsDecl, .reason = Reason::MissingTypeId},
        },
    });
}
