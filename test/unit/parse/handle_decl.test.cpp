// TEST_CASE_WITH_FIXTURE("Parse: Tuple decl", "[Parse][Decl]") {
//     assertDecl(
//         "(x: Int, y: $Float, (z, t))",
//         "decl::Tuple [@0:0] with size = 3\n"
//         "├─ decl::Identifier [@0:1] with name = 'x'\n"
//         "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Int')\n"
//         "├─ decl::Identifier [@0:9] with name = 'y'\n"
//         "   ├─ type::Identifier [@0:12] with (const, fund, path) = (false, true, 'Float')\n"
//         "├─ decl::Tuple [@0:20] with size = 2\n"
//         "   ├─ decl::Identifier [@0:21] with name = 'z'\n"
//         "   ├─ decl::Identifier [@0:24] with name = 't'"
//     );
// }
