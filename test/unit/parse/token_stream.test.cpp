#include <catch2/catch_test_macros.hpp>

#include "lex/lex.cpp"
#include "parse/token_stream.hpp"

class TokenStreamTestFixture {
protected:
private:
};

#define TEST_CASE_WITH_FIXTURE(...) \
TEST_CASE_METHOD(TokenStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("TokenStream:", "[Parse][TokenStream]") {}
