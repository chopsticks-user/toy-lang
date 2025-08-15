#include "static.test.hpp"
#include "static/post_parsing/tuple_reducer.hpp"

static constexpr auto mutator(tlc::syntax::Node& node) -> void {
    tlc::static_::SingletonTupleReducer::operator()(node);
}

TEST_CASE_WITH_FIXTURE(
    "Static.PostParsing.TupleReducer: Equivalence cases",
    "[Unit][Static][PostParsing]"
) {
    assertMutatorResult({
        .mutator = mutator,
        .source =
        "module foo;\n"
        "\n"
        "fn bar: () -> {\n"
        "   a := (((a+5)));\n"
        "   b := ((b) |> fun);\n"
        "   c: ((Int)) = 5+(a);\n"
        "}\n",

        // .expectedPrettyPrint =
        // "",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'foo'\n"
        "(empty)\n"
        "global::Function [@2:0] with visibility = ''\n"
        "├─ global::FunctionPrototype [@2:0] with name = 'bar'\n"
        "   ├─ (empty)\n"
        "   ├─ decl::Tuple [@2:8] with size = 0\n"
        "   ├─ (empty)\n"
        "├─ stmt::Block [@2:14] with size = 3\n"
        "   ├─ stmt::Assign [@3:3] with op = ':='\n"
        "      ├─ expr::Identifier [@3:3] with path = 'a'\n"
        "      ├─ expr::Binary [@3:11] with op = '+'\n"
        "         ├─ expr::Identifier [@3:11] with path = 'a'\n"
        "         ├─ expr::Integer [@3:13] with value = 5\n"
        "   ├─ stmt::Assign [@4:3] with op = ':='\n"
        "      ├─ expr::Identifier [@4:3] with path = 'b'\n"
        "      ├─ expr::Binary [@4:9] with op = '|>'\n"
        "         ├─ expr::Identifier [@4:10] with path = 'b'\n"
        "         ├─ expr::Identifier [@4:16] with path = 'fun'\n"
        "   ├─ stmt::Decl [@5:3]\n"
        "      ├─ decl::Identifier [@5:3] with name = 'c'\n"
        "         ├─ type::Identifier [@5:8] with (const, fund, path) = (true, true, 'Int')\n"
        "      ├─ expr::Binary [@5:16] with op = '+'\n"
        "         ├─ expr::Integer [@5:16] with value = 5\n"
        "         ├─ expr::Identifier [@5:19] with path = 'a'",
    });
}
