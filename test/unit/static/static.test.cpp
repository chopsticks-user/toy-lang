#include "static.test.hpp"

auto StaticTestFixture::assertTransformerResult(
    TransformerAssertionParams params, SLoc const location
) -> void {
    INFO(std::format("{}:{}", location.file_name(), location.line()));
    auto node = parse(std::move(params.source));
    params.transformer(node);

    params.expectedAstPrint.transform(
        [&node](auto&& expectedAstPrint) {
            auto const actualAstPrint =
                tlc::parse::ASTPrinter::operator()(node);
            REQUIRE(actualAstPrint == expectedAstPrint);
            return "";
        }
    );
}

#ifdef TLC_STATIC_POST_PARSING_A1_TUPLE_COLLAPSING_HPP
template auto StaticTestFixture::assertTransformerResult<A1TupleCollapsing>
(TransformerAssertionParams, SLoc) -> void;
#endif // TLC_STATIC_POST_PARSING_A1_TUPLE_COLLAPSING_HPP

auto StaticTestFixture::parse(tlc::Str source) -> tlc::syntax::Node {
    std::istringstream iss;
    iss.str(std::move(source));
    return tlc::parse::Parse{
        filepath, tlc::lex::Lex::operator()(std::move(iss))
    }();
}
