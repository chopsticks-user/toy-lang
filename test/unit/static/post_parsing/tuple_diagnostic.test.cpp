#include "static.test.hpp"
#include "static/post_parsing/tuple_diagnostic.hpp"

static constexpr auto visitor(tlc::syntax::Node const& node) -> void {
    std::ignore = tlc::static_::TupleDiagnostic::operator()(node);
}

TEST_CASE_WITH_FIXTURE(
    "Static.PostParsing.TupleDiagnostic: Equivalence cases",
    "[Unit][Static][PostParsing]"
) {
    auto const result = parse(
        "module foo;\n"
        "fn bar: () -> {\n"
        "   (a,b) := (5,6);\n"
        "}\n"
    );
    visitor(result);
}
