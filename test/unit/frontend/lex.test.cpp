#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

class LexTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexTestFixture, __VA_ARGS__)
