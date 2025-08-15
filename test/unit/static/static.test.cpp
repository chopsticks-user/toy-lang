#include "static.test.hpp"

auto StaticTestFixture::assertMutatorResult(
    TransformerAssertionParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    auto node = parse(std::move(params.source));
    params.mutator(node);

    params.expectedPrettyPrint.transform(
        [&node](auto&& expectedPrettyPrint) {
            auto const actualPrettyPrint =
                tlc::parse::PrettyPrint::operator()(node);
            REQUIRE(actualPrettyPrint == expectedPrettyPrint);
            return "";
        }
    );

    params.expectedAstPrint.transform(
        [&node](auto&& expectedAstPrint) {
            auto const actualAstPrint =
                tlc::parse::ASTPrinter::operator()(node);
            REQUIRE(actualAstPrint == expectedAstPrint);
            return "";
        }
    );
}

auto StaticTestFixture::parse(tlc::Str source) -> tlc::syntax::Node {
    std::istringstream iss;
    iss.str(std::move(source));
    return tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }();
}
