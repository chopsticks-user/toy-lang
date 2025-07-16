#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ParseTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: ", "[Parse]") {}
