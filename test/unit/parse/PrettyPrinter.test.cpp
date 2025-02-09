// #include <catch2/catch_test_macros.hpp>
//
// #include "FrontEnd/FrontEnd.hpp"
//
// #include <iostream>
//
// using tl::String;
// using namespace tl::syntax;
//
// #define TEST_CASE_WITH_FIXTURE(...) \
//   TEST_CASE_METHOD(PrettyPrinterTestFixture, __VA_ARGS__)
//
// #define REQUIRE_PRETTY_PRINTER(source, expected) \
//   REQUIRE(print(std::move(source)) == expected)
//
// class PrettyPrinterTestFixture {
// protected:
//   static constexpr auto filepath = "./prettyPrinterTestSample.toy";
//
//   auto print(String source) -> String {
//     std::istringstream iss;
//     iss.str(std::move(source));
//     const ASTNode ast = tl::apply<tl::fe::Parser>(
//       filepath, tl::apply<tl::fe::Lexer>(std::move(iss))
//     );
//     return tl::apply<tl::fe::PrettyPrinter>(astCast<TranslationUnit>(ast));
//   }
//
// private:
// };
//
// TEST_CASE_WITH_FIXTURE("PrettyPrinter: module, import and type declarations", "[PrettyPrinter]") {
//   String source = R"(
// module    foo ::bar;
// import std  ::io ;
// import  std:: math;
//
// export type   Number = math ::Complex   | Int| Float  ;
// type Complex=math::Complex;
// )";
//
//   String expected = R"(
// module foo::bar;
//
// import std::io;
// import std::math;
//
// export type Number = math::Complex | Int | Float;
//
// internal type Complex = math::Complex;
// )";
//
//   REQUIRE_PRETTY_PRINTER(source, expected);
// }
//
// TEST_CASE_WITH_FIXTURE("PrettyPrinter: concept", "[PrettyPrinter]") {
//   String source = R"(
// module    foo;
// import std  ::io ;
// import  std:: math;
//
// export type   Number = math ::Complex   | Int| Float  ;
// type Complex=math::Complex;
// )";
//
//   String expected = R"(
// module foo;
//
// import std::io;
// import std::math;
//
// export type Number = math::Complex | Int | Float;
//
// internal type Complex = math::Complex;
// )";
//
//   REQUIRE_PRETTY_PRINTER(source, expected);
// }
//
// TEST_CASE_WITH_FIXTURE("PrettyPrinter: function", "[PrettyPrinter]") {
//   String source = R"(
// module    foo;
// import std  ::io ;
// import  std:: math;
//
// export type   Number = math ::Complex   | Int| Float  ;
// type Complex=math::Complex;
// )";
//
//   String expected = R"(
// module foo;
//
// import std::io;
// import std::math;
//
// export type Number = math::Complex | Int | Float;
//
// internal type Complex = math::Complex;
// )";
//
//   REQUIRE_PRETTY_PRINTER(source, expected);
// }
