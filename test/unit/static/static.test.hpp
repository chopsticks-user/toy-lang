#ifndef TLC_TEST_UNIT_STATIC_TEST_HPP
#define TLC_TEST_UNIT_STATIC_TEST_HPP

#include <catch2/catch_test_macros.hpp>
#include <source_location>

#include "parse/parse.hpp"

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(StaticTestFixture, __VA_ARGS__)

class StaticTestFixture {
    using SLoc = std::source_location;

    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/static.toy";

protected:
    struct TransformerAssertionParams {
        void (*mutator)(tlc::syntax::Node&);
        tlc::Str source;
        tlc::Opt<tlc::Str> expectedPrettyPrint;
        tlc::Opt<tlc::Str> expectedAstPrint;
    };

protected:
    static auto assertMutatorResult(
        TransformerAssertionParams params, SLoc location = SLoc::current()
    ) -> void;

    static auto parse(tlc::Str source) -> tlc::syntax::Node;

private:
};

#endif // TLC_TEST_UNIT_STATIC_TEST_HPP
