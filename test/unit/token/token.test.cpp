#include <catch2/catch_test_macros.hpp>

#include "token/token.hpp"

class TokenTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(TokenTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Token: ", "[Token]") {}
