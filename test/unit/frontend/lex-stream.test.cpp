#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

class LexStreamTestFixture {
protected:
    static auto fromSource(tlc::Str source) -> tlc::lex::Stream {
        std::istringstream iss;
        iss.str(std::move(source));
        return tlc::lex::Stream{std::move(iss)};
    }

private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("LexStream: Sequential match", "[Lex]") {
    auto stream = fromSource(R"(
lex
        )");

    REQUIRE(stream.match('l'));
    REQUIRE(stream.match('e'));
    REQUIRE(stream.match('x'));
    REQUIRE_FALSE(stream.match('x')); // returns false at eof
}
