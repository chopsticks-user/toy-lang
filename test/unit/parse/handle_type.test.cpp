#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    type, Type, identifier, Identifier
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(type::Identifier);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(fundamental);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(imported);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(path);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    type, Type, tuple, Tuple
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(type::Tuple);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(size);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(children);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    type, Type, infer, Infer
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(type::Infer);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(expr);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    type, Type, array, Array
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(type::Array);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(dim);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(type);
    TLC_TEST_GENERATE_SELF_ASSERTION();
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    type, Type, function, Function
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(type::Function);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(args);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(result);
}

TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(type, Type, identifier, Identifier);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(type, Type, tuple, Tuple);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(type, Type, infer, Infer);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(type, Type, array, Array);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(type, Type, function, Function);

TEST_CASE_WITH_FIXTURE("Parse: Type identifiers", "[Parse]") {
    AssertType::identifier(
        "Int", {.fundamental = true, .imported = false, .path = "Int"}
    );

    AssertType::identifier(
        "foo::bar::Baz", {
            .fundamental = false, .imported = true, .path = "foo::bar::Baz"
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Type inference operator", "[Parse]") {
    AssertType::infer(
        "[[x]]", {
            .assert_expr = [](Node const& id) {
                AssertExpr::identifier(id, {"x"});
            }
        }
    );

    AssertType::infer(
        "[[3.14159]]", {
            .assert_expr = [](Node const& value) {
                AssertExpr::fl0at(value, {3.14159});
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Tuple types", "[Parse]") {
    AssertType::tuple("()", {.size = 0});

    AssertType::tuple(
        "(Foo)", {
            .size = 1,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = false, .imported = false, .path = "Foo"}
                );
            }
        }
    );

    AssertType::tuple(
        "(Int, Int)", {
            .size = 2,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[1],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
            }
        }
    );

    AssertType::tuple(
        "(Int, Float, Int, foo::Bar)", {
            .size = 4,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[1],
                    {.fundamental = true, .imported = false, .path = "Float"}
                );
                AssertType::identifier(
                    nodes[2],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[3],
                    {.fundamental = false, .imported = true, .path = "foo::Bar"}
                );
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Array types", "[Parse]") {
    AssertType::array(
        "Int[]", {
            .dim = 1,
            .assert_type = [](Node const& id) {
                AssertType::identifier(
                    id, {
                        .fundamental = true, .imported = false, .path = "Int"
                    }
                );
            },
            .assert_self = [](type::Array const& arr) {
                REQUIRE(isEmptyNode(arr.size(0)));
                REQUIRE_FALSE(arr.fixed(0));
            },
        }
    );

    AssertType::array(
        "[[x]][5]", {
            .dim = 1,
            .assert_type = [](Node const& type) {
                AssertType::infer(
                    type, {
                        [](Node const& expr) {
                            AssertExpr::identifier(expr, {"x"});
                        }
                    }
                );
            },
            .assert_self = [](type::Array const& arr) {
                REQUIRE_FALSE(isEmptyNode(arr.size(0)));
                REQUIRE(arr.fixed(0));
            },
        }
    );

    AssertType::array(
        "foo::Bar[][x + 1][][]", {
            .dim = 4,
            .assert_type = [](Node const& id) {
                AssertType::identifier(
                    id, {
                        .fundamental = false, .imported = true, .path = "foo::Bar"
                    }
                );
            },
            .assert_self = [](type::Array const& arr) {
                REQUIRE(isEmptyNode(arr.size(0)));
                REQUIRE_FALSE(arr.fixed(0));

                REQUIRE_FALSE(isEmptyNode(arr.size(1)));
                REQUIRE(arr.fixed(1));

                REQUIRE(isEmptyNode(arr.size(2)));
                REQUIRE_FALSE(arr.fixed(2));

                REQUIRE(isEmptyNode(arr.size(3)));
                REQUIRE_FALSE(arr.fixed(3));
            },
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function types", "[Parse]") {
    AssertType::function(
        "() ->", {
            .assert_args = [](Node const& args) {
                AssertType::tuple(args, {.size = 0});
            },
            .assert_result = [](Node const& result) {
                REQUIRE(isEmptyNode(result));
            }
        }
    );

    AssertType::function(
        "() -> ()", {
            .assert_args = [](Node const& args) {
                AssertType::tuple(args, {.size = 0});
            },
            .assert_result = [](Node const& result) {
                AssertType::tuple(result, {.size = 0});
            }
        }
    );

    AssertType::function(
        "Int -> Bool", {
            .assert_args = [](Node const& args) {
                AssertType::identifier(
                    args, {
                        .fundamental = true, .imported = false, .path = "Int"
                    }
                );
            },
            .assert_result = [](Node const& result) {
                AssertType::identifier(
                    result, {
                        .fundamental = true, .imported = false, .path = "Bool"
                    }
                );
            }
        }
    );

    AssertType::function(
        "(Int, Float) -> Bool", {
            .assert_args = [](Node const& args) {
                AssertType::tuple(
                    args, {
                        .size = 2,
                        .assert_children = [](tlc::Span<Node const> const nodes) {
                            AssertType::identifier(
                                nodes[0], {
                                    .fundamental = true, .imported = false, .path = "Int"
                                }
                            );
                            AssertType::identifier(
                                nodes[1], {
                                    .fundamental = true, .imported = false, .path = "Float"
                                }
                            );
                        }
                    }
                );
            },
            .assert_result = [](Node const& result) {
                AssertType::identifier(
                    result, {
                        .fundamental = true, .imported = false, .path = "Bool"
                    }
                );
            }
        }
    );

    AssertType::function(
        "(Int, foo::Bar) -> (Int, Bool)", {
            .assert_args = [](Node const& args) {
                AssertType::tuple(
                    args, {
                        .size = 2,
                        .assert_children = [](tlc::Span<Node const> const nodes) {
                            AssertType::identifier(
                                nodes[0], {
                                    .fundamental = true, .imported = false,
                                    .path = "Int"
                                }
                            );
                            AssertType::identifier(
                                nodes[1], {
                                    .fundamental = false, .imported = true,
                                    .path = "foo::Bar"
                                }
                            );
                        }
                    }
                );
            },
            .assert_result = [](Node const& result) {
                AssertType::tuple(
                    result, {
                        .size = 2,
                        .assert_children = [](tlc::Span<Node const> const nodes) {
                            AssertType::identifier(
                                nodes[0], {
                                    .fundamental = true, .imported = false,
                                    .path = "Int"
                                }
                            );
                            AssertType::identifier(
                                nodes[1], {
                                    .fundamental = true, .imported = false,
                                    .path = "Bool"
                                }
                            );
                        }
                    }
                );
            }
        }
    );

    AssertType::function("Int -> Float -> Bool", {});
}
