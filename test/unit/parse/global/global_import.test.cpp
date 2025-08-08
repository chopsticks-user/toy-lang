#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Import: Empty",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module global;\n"
        "import;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'global'\n"
        "global::ImportDeclGroup [@1:0] with size = 1\n"
        "├─ global::ImportDecl [@1:0]\n"
        "   ├─ (required)\n"
        "   ├─ (empty)",

        .expectedPrettyPrint =
        "module global;\n"
        "\n"
        "import {!};",

        .expectedErrors = {
            {.context = Context::ImportDecl, .reason = Reason::MissingId},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Import: 1-level imported path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module global;\n"
        "import foo;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'global'\n"
        "global::ImportDeclGroup [@1:0] with size = 1\n"
        "├─ global::ImportDecl [@1:0]\n"
        "   ├─ expr::Identifier [@1:7] with path = 'foo'\n"
        "   ├─ (empty)",

        .expectedPrettyPrint =
        "module global;\n"
        "\n"
        "import foo;",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Import: 3-level imported path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module global;\n"
        "import foo.bar.baz;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'global'\n"
        "global::ImportDeclGroup [@1:0] with size = 1\n"
        "├─ global::ImportDecl [@1:0]\n"
        "   ├─ expr::Identifier [@1:7] with path = 'foo.bar.baz'\n"
        "   ├─ (empty)",

        .expectedPrettyPrint =
        "module global;\n"
        "\n"
        "import foo.bar.baz;",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Import: 1-level alias path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module global;\n"
        "import foo = bar.baz;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'global'\n"
        "global::ImportDeclGroup [@1:0] with size = 1\n"
        "├─ global::ImportDecl [@1:0]\n"
        "   ├─ expr::Identifier [@1:7] with path = 'foo'\n"
        "   ├─ expr::Identifier [@1:13] with path = 'bar.baz'",

        .expectedPrettyPrint =
        "module global;\n"
        "\n"
        "import foo = bar.baz;",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Import: 2-level alias path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module global;\n"
        "import foo.bar = foo.bar.baz;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'global'\n"
        "global::ImportDeclGroup [@1:0] with size = 1\n"
        "├─ global::ImportDecl [@1:0]\n"
        "   ├─ expr::Identifier [@1:7] with path = 'foo.bar'\n"
        "   ├─ expr::Identifier [@1:17] with path = 'foo.bar.baz'",

        .expectedPrettyPrint =
        "module global;\n"
        "\n"
        "import foo.bar = foo.bar.baz;",
    });
}
