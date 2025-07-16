#include <catch2/catch_test_macros.hpp>

#include "syntax/syntax.hpp"

class SyntaxTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(SyntaxTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Syntax: ", "[Syntax]") {}
