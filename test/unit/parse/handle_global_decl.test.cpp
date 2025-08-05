#include "parse.test.hpp"

using namespace tlc::syntax;

TEST_CASE_WITH_FIXTURE("Parse: Module declarations", "[Parse][GlobalDecl]") {
    assertTranslationUnit(
        "module",
        ""
    );
    assertTranslationUnit(
        "module foo",
        "├─ global::ModuleDecl [@0:0]\n"
        "   ├─ expr::Identifier [@0:7] with path = 'foo'"
    );
    assertTranslationUnit(
        "module foo.bar.baz",
        "├─ global::ModuleDecl [@0:0]\n"
        "   ├─ expr::Identifier [@0:7] with path = 'foo.bar.baz'"
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Import declarations", "[Parse][GlobalDecl]") {
    assertTranslationUnit(
        R"(
module root

import foo
import foo.bar
import foo = bar.baz
import foo.bar = foo.bar.baz
)",
        "├─ global::ModuleDecl [@1:0]\n"
        "   ├─ expr::Identifier [@1:7] with path = 'root'\n"
        "├─ global::ImportDecl [@3:0]\n"
        "   ├─ expr::Identifier [@3:7] with path = 'foo'\n"
        "├─ global::ImportDecl [@4:0]\n"
        "   ├─ expr::Identifier [@4:7] with path = 'foo.bar'\n"
        "├─ global::ImportDecl [@5:0]\n"
        "   ├─ expr::Identifier [@5:7] with path = 'foo'\n"
        "   ├─ expr::Identifier [@5:13] with path = 'bar.baz'\n"
        "├─ global::ImportDecl [@6:0]\n"
        "   ├─ expr::Identifier [@6:7] with path = 'foo.bar'\n"
        "   ├─ expr::Identifier [@6:17] with path = 'foo.bar.baz'"
    );
}
