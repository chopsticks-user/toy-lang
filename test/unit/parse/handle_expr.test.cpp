// #include "parse.test.hpp"
//
// TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Unit][Parse][Expr]") {
//     assertExpr(
//         "foo.bar(0,1,2)",
//         "expr::FnApp [@0:0]\n"
//         "├─ expr::Identifier [@0:0] with path = 'foo.bar'\n"
//         "├─ expr::Tuple [@0:7] with size = 3\n"
//         "   ├─ expr::Integer [@0:8] with value = 0\n"
//         "   ├─ expr::Integer [@0:10] with value = 1\n"
//         "   ├─ expr::Integer [@0:12] with value = 2"
//     );
//     assertExpr(
//         "foo()",
//         "expr::FnApp [@0:0]\n"
//         "├─ expr::Identifier [@0:0] with path = 'foo'\n"
//         "├─ expr::Tuple [@0:3] with size = 0"
//     );
//     assertExpr(
//         "foo(0,1,2)(bar)",
//         "expr::FnApp [@0:0]\n"
//         "├─ expr::FnApp [@0:0]\n"
//         "   ├─ expr::Identifier [@0:0] with path = 'foo'\n"
//         "   ├─ expr::Tuple [@0:3] with size = 3\n"
//         "      ├─ expr::Integer [@0:4] with value = 0\n"
//         "      ├─ expr::Integer [@0:6] with value = 1\n"
//         "      ├─ expr::Integer [@0:8] with value = 2\n"
//         "├─ expr::Tuple [@0:10] with size = 1\n"
//         "   ├─ expr::Identifier [@0:11] with path = 'bar'"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Unit][Parse][Expr]") {
//     assertExpr(
//         "foo.bar[0,1,2]",
//         "expr::Subscript [@0:0]\n"
//         "├─ expr::Identifier [@0:0] with path = 'foo.bar'\n"
//         "├─ expr::Array [@0:7] with size = 3\n"
//         "   ├─ expr::Integer [@0:8] with value = 0\n"
//         "   ├─ expr::Integer [@0:10] with value = 1\n"
//         "   ├─ expr::Integer [@0:12] with value = 2"
//     );
//     assertExpr(
//         "foo[x]",
//         "expr::Subscript [@0:0]\n"
//         "├─ expr::Identifier [@0:0] with path = 'foo'\n"
//         "├─ expr::Array [@0:3] with size = 1\n"
//         "   ├─ expr::Identifier [@0:4] with path = 'x'"
//     );
//     assertExpr(
//         "foo[0,1,2][bar]",
//         "expr::Subscript [@0:0]\n"
//         "├─ expr::Subscript [@0:0]\n"
//         "   ├─ expr::Identifier [@0:0] with path = 'foo'\n"
//         "   ├─ expr::Array [@0:3] with size = 3\n"
//         "      ├─ expr::Integer [@0:4] with value = 0\n"
//         "      ├─ expr::Integer [@0:6] with value = 1\n"
//         "      ├─ expr::Integer [@0:8] with value = 2\n"
//         "├─ expr::Array [@0:10] with size = 1\n"
//         "   ├─ expr::Identifier [@0:11] with path = 'bar'"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Prefix expressions", "[Unit][Parse][Expr]") {
//     assertExpr(
//         "!x",
//         "expr::Prefix [@0:0] with op = '!'\n"
//         "├─ expr::Identifier [@0:1] with path = 'x'"
//     );
//     assertExpr(
//         "!!!x",
//         "expr::Prefix [@0:0] with op = '!'\n"
//         "├─ expr::Prefix [@0:1] with op = '!'\n"
//         "   ├─ expr::Prefix [@0:2] with op = '!'\n"
//         "      ├─ expr::Identifier [@0:3] with path = 'x'"
//     );
//     assertExpr(
//         "...[0,1,2]",
//         "expr::Prefix [@0:0] with op = '...'\n"
//         "├─ expr::Array [@0:3] with size = 3\n"
//         "   ├─ expr::Integer [@0:4] with value = 0\n"
//         "   ├─ expr::Integer [@0:6] with value = 1\n"
//         "   ├─ expr::Integer [@0:8] with value = 2"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Binary expressions", "[Unit][Parse][Expr]") {
//     assertExpr(
//         "-x+3",
//         "expr::Binary [@0:0] with op = '+'\n"
//         "├─ expr::Prefix [@0:0] with op = '-'\n"
//         "   ├─ expr::Identifier [@0:1] with path = 'x'\n"
//         "├─ expr::Integer [@0:3] with value = 3"
//     );
//     assertExpr(
//         "-x+y*z",
//         "expr::Binary [@0:0] with op = '+'\n"
//         "├─ expr::Prefix [@0:0] with op = '-'\n"
//         "   ├─ expr::Identifier [@0:1] with path = 'x'\n"
//         "├─ expr::Binary [@0:3] with op = '*'\n"
//         "   ├─ expr::Identifier [@0:3] with path = 'y'\n"
//         "   ├─ expr::Identifier [@0:5] with path = 'z'"
//     );
//     assertExpr(
//         "x*-y+z",
//         "expr::Binary [@0:0] with op = '+'\n"
//         "├─ expr::Binary [@0:0] with op = '*'\n"
//         "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
//         "   ├─ expr::Prefix [@0:2] with op = '-'\n"
//         "      ├─ expr::Identifier [@0:3] with path = 'y'\n"
//         "├─ expr::Identifier [@0:5] with path = 'z'"
//     );
// }
//
// // todo
// TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Unit][Parse][Expr]") {}
//
// // todo
// TEST_CASE_WITH_FIXTURE("Parse: Operator precedence", "[Unit][Parse][Expr]") {}
