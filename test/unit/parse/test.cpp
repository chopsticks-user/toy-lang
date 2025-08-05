#include <catch2/catch_test_macros.hpp>
#include <core/core.hpp>

struct Test : tlc::MoveOnly<Test> {
    // Test(Test&&) = delete;
};

struct Test1 : tlc::Copyable<Test1> {
    // Test(Test&&) = delete;
};

TEST_CASE("Exp", "[Exp]") {
    STATIC_REQUIRE(std::is_move_constructible_v<Test1>);
    STATIC_REQUIRE(std::is_move_assignable_v<Test1>);
    STATIC_REQUIRE(std::is_default_constructible_v<Test1>);
}
