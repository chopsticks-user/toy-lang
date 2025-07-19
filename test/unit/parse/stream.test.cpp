#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"
#include "parse/stream.hpp"

class ParseStreamTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(ParseStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("ParseStream:", "[Parse]") {}
