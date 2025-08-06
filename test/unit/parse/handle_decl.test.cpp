// #include "parse.test.hpp"
//
// using namespace tlc::syntax;
//
// auto ParseTestFixture::assertDecl(
//     tlc::Str source, tlc::Str expected, std::source_location const location
// ) -> void {
//     INFO(std::format("{}:{}", location.file_name(), location.line()));
//     std::istringstream iss;
//     iss.str(std::move(source));
//
//     auto result = tlc::parse::Parse{
//         filepath, tlc::lex::Lex::operator()(std::move(iss))
//     }.parseDecl();
//     REQUIRE(result.has_value());
//
//     auto const actual = tlc::parse::ASTPrinter::operator()(
//         std::move(*result)
//     );
//     REQUIRE(actual == expected);
// }
//
// auto ParseTestFixture::assertGenericParamsDecl(
//     tlc::Str source, tlc::Str expected,
//     std::source_location location
// ) -> void {
//     INFO(std::format("{}:{}", location.file_name(), location.line()));
//     std::istringstream iss;
//     iss.str(std::move(source));
//
//     auto result = tlc::parse::Parse{
//         filepath, tlc::lex::Lex::operator()(std::move(iss))
//     }.parseGenericParamsDecl();
//     REQUIRE(result.has_value());
//
//     auto const actual = tlc::parse::ASTPrinter::operator()(
//         std::move(*result)
//     );
//     REQUIRE(actual == expected);
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Identifier decl", "[Parse][Decl]") {
//     assertDecl(
//         "x: Foo",
//         "decl::Identifier [@0:0] with name = 'x'\n"
//         "├─ type::Identifier [@0:3] with (const, fund, path) = (true, false, 'Foo')"
//     );
//     assertDecl(
//         "x: $Float",
//         "decl::Identifier [@0:0] with name = 'x'\n"
//         "├─ type::Identifier [@0:3] with (const, fund, path) = (false, true, 'Float')"
//     );
//     assertDecl(
//         "x",
//         "decl::Identifier [@0:0] with name = 'x'"
//     );
// }
//
// TEST_CASE_WITH_FIXTURE("Parse: Tuple decl", "[Parse][Decl]") {
//     assertDecl(
//         "()",
//         "decl::Tuple [@0:0] with size = 0"
//     );
//     assertDecl(
//         "(x: Int, y: Float, z)",
//         "decl::Tuple [@0:0] with size = 3\n"
//         "├─ decl::Identifier [@0:1] with name = 'x'\n"
//         "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Int')\n"
//         "├─ decl::Identifier [@0:9] with name = 'y'\n"
//         "   ├─ type::Identifier [@0:12] with (const, fund, path) = (true, true, 'Float')\n"
//         "├─ decl::Identifier [@0:19] with name = 'z'"
//     );
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
//
// TEST_CASE_WITH_FIXTURE("Parse: Generic params decl", "[Parse][Decl]") {
//     assertGenericParamsDecl(
//         "<>",
//         "decl::GenericParameters [@0:0] with size = 0"
//     );
//     assertGenericParamsDecl(
//         "<T, U, V>",
//         "decl::GenericParameters [@0:0] with size = 3\n"
//         "├─ decl::GenericIdentifier [@0:1] with name = 'T'\n"
//         "├─ decl::GenericIdentifier [@0:4] with name = 'U'\n"
//         "├─ decl::GenericIdentifier [@0:7] with name = 'V'"
//     );
// }
