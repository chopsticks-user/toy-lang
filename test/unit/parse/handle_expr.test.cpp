#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, identifier, Identifier
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Identifier);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(path);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, integer, Integer
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Integer);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(value);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, fl0at, Float
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Float);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(value);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, boolean, Boolean
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Boolean);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(value);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, tuple, Tuple
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Tuple);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(children);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, array, Array
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Array);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(children);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, access, Access
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Access);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(object);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(field);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, fnApp, FnApp
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::FnApp);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(callee);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(args);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, subscript, Subscript
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Subscript);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(collection);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(subscript);
}

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    expr, Expr, prefix, Prefix
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(expr::Prefix);
    TLC_TEST_GENERATE_COMPARE_ASSERTION(op);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(operand);
}

TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, identifier, Identifier);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, integer, Integer);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, fl0at, Float);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, boolean, Boolean);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, tuple, Tuple);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, array, Array);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, access, Access);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, fnApp, FnApp);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, subscript, Subscript);
TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(expr, Expr, prefix, Prefix);

TEST_CASE_WITH_FIXTURE("Parse: Integers", "[Parse]") {
    SECTION("Base 10") {
        AssertExpr::integer("31415", {31415});
        AssertExpr::integer("0", {0});
    }

    SECTION("Base 16") {
        AssertExpr::integer("0x123456789abcdef", {0x123456789abcdef});
        AssertExpr::integer("0x0", {0});
    }

    SECTION("Base 8") {
        AssertExpr::integer("01234567", {01234567});
        AssertExpr::integer("00", {0});
    }

    SECTION("Base 2") {
        AssertExpr::integer("0b1010101001", {0b1010101001});
        AssertExpr::integer("0b0", {0});
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Floats", "[Parse]") {
    AssertExpr::fl0at("0.0314159", {0.0314159});
    AssertExpr::fl0at("3145.1926", {3145.1926});
    AssertExpr::fl0at("0.314159", {0.314159});
    AssertExpr::fl0at("00.31415", {0.31415});
    AssertExpr::fl0at("000.3141", {0.3141});
    AssertExpr::fl0at("0.000", {0.0});
    AssertExpr::fl0at("00.00", {0.0});
    AssertExpr::fl0at("000.0", {0.0});
}

TEST_CASE_WITH_FIXTURE("Parse: Booleans", "[Parse]") {
    AssertExpr::boolean("true", {true});
    AssertExpr::boolean("false", {false});
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    SECTION("Local identifier") {
        AssertExpr::identifier("baz", {"baz"});
    }

    SECTION("Imported identifier") {
        AssertExpr::identifier("foo::bar", {"foo::bar"});
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Tuples", "[Parse]") {
    AssertExpr::tuple(
        "(1,x,2.1)", {
            3, [](tlc::Span<Node const> const elements) {
                AssertExpr::integer(elements[0], {1});
                AssertExpr::identifier(elements[1], {"x"});
                AssertExpr::fl0at(elements[2], {2.1});
            }
        }
    );

    AssertExpr::tuple(
        "(0)", {
            1, [](tlc::Span<Node const> const elements) {
                AssertExpr::integer(elements[0], {0});
            }
        }
    );

    AssertExpr::tuple("()", {0});
}

TEST_CASE_WITH_FIXTURE("Parse: Arrays", "[Parse]") {
    AssertExpr::array(
        "[1,x,2.1]", {
            3, [](tlc::Span<Node const> const elements) {
                AssertExpr::integer(elements[0], {1});
                AssertExpr::identifier(elements[1], {"x"});
                AssertExpr::fl0at(elements[2], {2.1});
            }
        }
    );

    AssertExpr::array(
        "[0]", {
            1, [](tlc::Span<Node const> const elements) {
                AssertExpr::integer(elements[0], {0});
            }
        }
    );

    AssertExpr::array("[]", {0});
}

TEST_CASE_WITH_FIXTURE("Parse: Records", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Access expressions", "[Parse]") {
    AssertExpr::access(
        "foo.bar", {
            [](Node const& object) {
                AssertExpr::identifier(object, {"foo"});
            },
            [](Node const& field) {
                AssertExpr::identifier(field, {"bar"});
            }
        }
    );

    AssertExpr::access(
        "[1,2,3].foo", {
            [](Node const& object) {
                AssertExpr::array(
                    object, {
                        3, [](tlc::Span<Node const> elements) {
                            AssertExpr::integer(elements[0], {1});
                            AssertExpr::integer(elements[1], {2});
                            AssertExpr::integer(elements[2], {3});
                        }
                    }
                );
            },
            [](Node const& field) {
                AssertExpr::identifier(field, {"foo"});
            }
        }
    );

    AssertExpr::access(
        "foo.bar.baz", {
            [](Node const& object) {
                AssertExpr::access(
                    object, {
                        [](Node const& object1) {
                            AssertExpr::identifier(object1, {"foo"});
                        },
                        [](Node const& field1) {
                            AssertExpr::identifier(field1, {"bar"});
                        }
                    }
                );
            },
            [](Node const& field) {
                AssertExpr::identifier(field, {"baz"});
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Parse]") {
    AssertExpr::fnApp(
        "foo(0,1,2)", {
            [](Node const& callee) {
                AssertExpr::identifier(callee, {"foo"});
            },
            [](Node const& args) {
                AssertExpr::tuple(
                    args, {
                        3, [](tlc::Span<Node const> const elements) {
                            AssertExpr::integer(elements[0], {0});
                            AssertExpr::integer(elements[1], {1});
                            AssertExpr::integer(elements[2], {2});
                        }
                    }
                );
            }
        }
    );

    AssertExpr::fnApp(
        "foo()", {
            [](Node const& callee) {
                AssertExpr::identifier(callee, {"foo"});
            },
            [](Node const& args) {
                AssertExpr::tuple(args, {0});
            }
        }
    );

    AssertExpr::fnApp(
        "foo(0,1,2)(bar)", {
            [](Node const& callee) {
                AssertExpr::fnApp(
                    callee, {
                        [](Node const& callee1) {
                            AssertExpr::identifier(callee1, {"foo"});
                        },
                        [](Node const& args1) {
                            AssertExpr::tuple(
                                args1, {
                                    3, [](tlc::Span<Node const> const elements) {
                                        AssertExpr::integer(elements[0], {0});
                                        AssertExpr::integer(elements[1], {1});
                                        AssertExpr::integer(elements[2], {2});
                                    }
                                }
                            );
                        }
                    }
                );
            },
            [](Node const& args) {
                AssertExpr::tuple(
                    args, {
                        1,
                        [](tlc::Span<Node const> const elements) {
                            AssertExpr::identifier(
                                elements[0], {"bar"}
                            );
                        }
                    }
                );
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Parse]") {
    AssertExpr::subscript(
        "foo[0,1,2]", {
            [](Node const& callee) {
                AssertExpr::identifier(callee, {"foo"});
            },
            [](Node const& args) {
                AssertExpr::array(
                    args, {
                        3,
                        [](tlc::Span<Node const> const elements) {
                            AssertExpr::integer(elements[0], {0});
                            AssertExpr::integer(elements[1], {1});
                            AssertExpr::integer(elements[2], {2});
                        }
                    }
                );
            }
        }
    );

    // todo: error
    AssertExpr::subscript(
        "foo[]", {
            [](Node const& callee) {
                AssertExpr::identifier(callee, {"foo"});
            },
            [](Node const& args) {
                AssertExpr::array(args, {0});
            }
        }
    );

    AssertExpr::subscript(
        "foo[0,1,2][bar]", {
            [](Node const& callee) {
                AssertExpr::subscript(
                    callee, {
                        [](Node const& callee1) {
                            AssertExpr::identifier(callee1, {"foo"});
                        },
                        [](Node const& args1) {
                            AssertExpr::array(
                                args1, {
                                    3, [](tlc::Span<Node const> const elements) {
                                        AssertExpr::integer(elements[0], {0});
                                        AssertExpr::integer(elements[1], {1});
                                        AssertExpr::integer(elements[2], {2});
                                    }
                                }
                            );
                        }
                    }
                );
            },
            [](Node const& args) {
                AssertExpr::array(
                    args, {
                        1, [](tlc::Span<Node const> const elements) {
                            AssertExpr::identifier(
                                elements[0], {"bar"}
                            );
                        }
                    }
                );
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Prefix expressions", "[Parse]") {
    AssertExpr::prefix(
        "!5", {
            EToken::Exclaim, [](Node const& operand) {
                AssertExpr::integer(operand, {5});
            }
        }
    );

    AssertExpr::prefix(
        "!!!5", {
            EToken::Exclaim, [](Node const& operand) {
                AssertExpr::prefix(
                    operand, {
                        EToken::Exclaim, [](Node const& operand1) {
                            AssertExpr::prefix(
                                operand1, {
                                    EToken::Exclaim, [](Node const& operand2) {
                                        AssertExpr::integer(operand2, {5});
                                    }
                                }
                            );
                        }
                    }
                );
            }
        }
    );

    AssertExpr::prefix(
        "...[0,1,2]", {
            EToken::Dot3, [](Node const& operand) {
                AssertExpr::array(
                    operand, {
                        3, [](tlc::Span<Node const> const elements) {
                            AssertExpr::integer(elements[0], {0});
                            AssertExpr::integer(elements[1], {1});
                            AssertExpr::integer(elements[2], {2});
                        }
                    }
                );
            }
        }
    );

    AssertExpr::prefix(
        "...[0,1,2]", {
            EToken::Dot3, [](Node const& operand) {
                AssertExpr::array(
                    operand, {
                        3, [](tlc::Span<Node const> const elements) {
                            AssertExpr::integer(elements[0], {0});
                            AssertExpr::integer(elements[1], {1});
                            AssertExpr::integer(elements[2], {2});
                        }
                    }
                );
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Binary expressions", "[Parse]") {
    SECTION("Simple") {
        auto const expr =
            parseExpr<expr::Binary>("-x+3");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Prefix>(expr.left());
        REQUIRE(left.op() == EToken::Minus);
        AssertExpr::identifier(
            left.operand(), {"x"}
        );

        AssertExpr::integer(expr.right(), {3});
    }

    SECTION("Prioritize right") {
        auto const expr =
            parseExpr<expr::Binary>("-x+y*z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Prefix>(expr.left());
        REQUIRE(left.op() == EToken::Minus);
        AssertExpr::identifier(
            left.operand(), {"x"}
        );

        auto const right =
            cast<expr::Binary>(expr.right());
        REQUIRE(right.op() == EToken::Star);
        AssertExpr::identifier(
            right.left(), {"y"}
        );
        AssertExpr::identifier(
            right.right(), {"z"}
        );
    }

    SECTION("Prioritize left") {
        auto const expr =
            parseExpr<expr::Binary>("x*-y+z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Binary>(expr.left());
        REQUIRE(left.op() == EToken::Star);
        AssertExpr::identifier(
            left.left(), {"x"}
        );
        auto const leftRight =
            cast<expr::Prefix>(left.right());
        REQUIRE(leftRight.op() == EToken::Minus);
        AssertExpr::identifier(
            leftRight.operand(), {"y"}
        );

        AssertExpr::identifier(
            expr.right(), {"z"}
        );
    }
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}
