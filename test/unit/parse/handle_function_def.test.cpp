// #include "parse.test.hpp"
//
// using namespace tlc::syntax;
//
// TEST_CASE_WITH_FIXTURE("Parse: Public, trivial", "[Parse][Function]") {
//     assertTranslationUnit(
//         R"(
// module function;
//
// pub fn foo: () -> {}
// )",
//         "├─ global::ModuleDecl [@1:0]\n"
//         "   ├─ expr::Identifier [@1:7] with path = 'function'\n"
//         "├─ global::Function [@3:0] with visibility = 'pub'\n"
//         "   ├─ global::FunctionPrototype [@3:4] with name = 'foo'\n"
//         "      ├─ decl::Tuple [@3:12] with size = 0\n"
//         "   ├─ stmt::Block [@3:18] with size = 0"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Private, trivial", "[Parse][Function]") {
//     assertTranslationUnit(
//         R"(
// module function;
//
// prv fn foo: () -> {}
// )",
//         "├─ global::ModuleDecl [@1:0]\n"
//         "   ├─ expr::Identifier [@1:7] with path = 'function'\n"
//         "├─ global::Function [@3:0] with visibility = 'prv'\n"
//         "   ├─ global::FunctionPrototype [@3:4] with name = 'foo'\n"
//         "      ├─ decl::Tuple [@3:12] with size = 0\n"
//         "   ├─ stmt::Block [@3:18] with size = 0"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Local, trivial", "[Parse][Function]") {
//     assertTranslationUnit(
//         R"(
// module function;
//
// fn foo: () -> {}
// )",
//         "├─ global::ModuleDecl [@1:0]\n"
//         "   ├─ expr::Identifier [@1:7] with path = 'function'\n"
//         "├─ global::Function [@3:0] with visibility = ''\n"
//         "   ├─ global::FunctionPrototype [@3:0] with name = 'foo'\n"
//         "      ├─ decl::Tuple [@3:8] with size = 0\n"
//         "   ├─ stmt::Block [@3:14] with size = 0"
//     );
// }
//
//
// TEST_CASE_WITH_FIXTURE("Parse: 3 parameters, inferred return", "[Parse][Function]") {
//     //     assertTranslationUnit(
//     //         R"(
//     // module function;
//     //
//     // fn foo: (x: Int, y: Float, z: bar.Baz) -> {}
//     // )",
//     //         ""
//     //     );
// }
