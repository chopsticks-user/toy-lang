#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;


TEST_CASE_WITH_FIXTURE("Parse: Stmt", "[Parse]") {
    /**
     *  let x: Int = {
     *      yield 2;
     *  };
     */
    [[maybe_unused]] int64_t x = [&] { return 2; }();
}
