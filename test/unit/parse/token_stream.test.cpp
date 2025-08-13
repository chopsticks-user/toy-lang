#include <catch2/catch_test_macros.hpp>

import parse;

class TokenStreamTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(TokenStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("TokenStream:", "[Parse][TokenStream]") {}
