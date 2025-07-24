#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

TEST_CASE_WITH_FIXTURE("Parse: Integers", "[Parse]") {
    SECTION("Base 10") {
        assertInteger("31415", 31415);
        assertInteger("0", 0);
    }

    SECTION("Base 16") {
        assertInteger("0x123456789abcdef", 0x123456789abcdef);
        assertInteger("0x0", 0);
    }

    SECTION("Base 8") {
        assertInteger("01234567", 01234567);
        assertInteger("00", 0);
    }

    SECTION("Base 2") {
        assertInteger("0b1010101001", 0b1010101001);
        assertInteger("0b0", 0);
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Floats", "[Parse]") {
    assertFloat("0.0314159", 0.0314159);
    assertFloat("3145.1926", 3145.1926);
    assertFloat("0.314159", 0.314159);
    assertFloat("00.31415", 0.31415);
    assertFloat("000.3141", 0.3141);
    assertFloat("0.000", 0.0);
    assertFloat("00.00", 0.0);
    assertFloat("000.0", 0.0);
}

TEST_CASE_WITH_FIXTURE("Parse: Booleans", "[Parse]") {
    assertBoolean("true", true);
    assertBoolean("false", false);
}

// todo:
TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    SECTION("Local identifier") {
        assertIdentifier(
            "baz", EToken::Identifier, "baz"
        );
    }

    SECTION("Imported identifier") {
        assertIdentifier(
            "foo::bar", EToken::Identifier, "foo::bar"
        );
    }

    SECTION("Imported type") {
        assertIdentifier(
            "foo::bar::Baz", EToken::UserDefinedType, "foo::bar::Baz"
        );
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Tuples", "[Parse]") {
    assertTuple(
        "(1,x,2.1)", 3, [](tlc::Span<Node const> const elements) {
            assertInteger(elements[0], 1);
            assertIdentifier(elements[1], EToken::Identifier, "x");
            assertFloat(elements[2], 2.1);
        }
    );

    assertTuple(
        "(0)", 1, [](tlc::Span<Node const> const elements) {
            assertInteger(elements[0], 0);
        }
    );

    assertTuple("()", 0);
}

TEST_CASE_WITH_FIXTURE("Parse: Arrays", "[Parse]") {
    assertArray(
        "[1,x,2.1]", 3, [](tlc::Span<Node const> const elements) {
            assertInteger(elements[0], 1);
            assertIdentifier(elements[1], EToken::Identifier, "x");
            assertFloat(elements[2], 2.1);
        }
    );

    assertArray(
        "[0]", 1, [](tlc::Span<Node const> const elements) {
            assertInteger(elements[0], 0);
        }
    );

    assertArray("[]", 0);
}

TEST_CASE_WITH_FIXTURE("Parse: Access expressions", "[Parse]") {
    assertAccessExpr(
        "foo.bar",
        [](Node const& object) {
            assertIdentifier(object, EToken::Identifier, "foo");
        },
        [](Node const& field) {
            assertIdentifier(field, EToken::Identifier, "bar");
        }
    );

    assertAccessExpr(
        "foo.Bar",
        [](Node const& object) {
            assertIdentifier(object, EToken::Identifier, "foo");
        },
        [](Node const& field) {
            assertIdentifier(field, EToken::UserDefinedType, "Bar");
        }
    );

    assertAccessExpr(
        "Foo.Bar",
        [](Node const& object) {
            assertIdentifier(object, EToken::UserDefinedType, "Foo");
        },
        [](Node const& field) {
            assertIdentifier(field, EToken::UserDefinedType, "Bar");
        }
    );

    assertAccessExpr(
        "[1,2,3].foo",
        [](Node const& object) {
            assertArray(object, 3, [](tlc::Span<Node const> elements) {
                assertInteger(elements[0], 1);
                assertInteger(elements[1], 2);
                assertInteger(elements[2], 3);
            });
        },
        [](Node const& field) {
            assertIdentifier(field, EToken::Identifier, "foo");
        }
    );

    assertAccessExpr(
        "foo.bar.baz",
        [](Node const& object) {
            assertAccessExpr(
                object,
                [](Node const& object1) {
                    assertIdentifier(object1, EToken::Identifier, "foo");
                },
                [](Node const& field1) {
                    assertIdentifier(field1, EToken::Identifier, "bar");
                }
            );
        },
        [](Node const& field) {
            assertIdentifier(field, EToken::Identifier, "baz");
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Parse]") {
    assertFnAppExpr(
        "foo(0,1,2)",
        [](Node const& callee) {
            assertIdentifier(callee, EToken::Identifier, "foo");
        }, [](Node const& args) {
            assertTuple(args, 3, [](tlc::Span<Node const> const elements) {
                assertInteger(elements[0], 0);
                assertInteger(elements[1], 1);
                assertInteger(elements[2], 2);
            });
        }
    );

    assertFnAppExpr(
        "foo()",
        [](Node const& callee) {
            assertIdentifier(callee, EToken::Identifier, "foo");
        }, [](Node const& args) {
            assertTuple(args, 0);
        }
    );

    assertFnAppExpr(
        "foo(0,1,2)(bar)",
        [](Node const& callee) {
            assertFnAppExpr(
                callee,
                [](Node const& callee1) {
                    assertIdentifier(callee1, EToken::Identifier, "foo");
                }
                ,
                [](Node const& args1) {
                    assertTuple(
                        args1, 3,
                        [](tlc::Span<Node const> const elements) {
                            assertInteger(elements[0], 0);
                            assertInteger(elements[1], 1);
                            assertInteger(elements[2], 2);
                        });
                }
            );
        }, [](Node const& args) {
            assertTuple(
                args, 1,
                [](tlc::Span<Node const> const elements) {
                    assertIdentifier(
                        elements[0], EToken::Identifier, "bar"
                    );
                });
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Parse]") {
    assertSubscriptExpr(
        "foo[0,1,2]",
        [](Node const& callee) {
            assertIdentifier(callee, EToken::Identifier, "foo");
        }, [](Node const& args) {
            assertArray(
                args, 3,
                [](tlc::Span<Node const> const elements) {
                    assertInteger(elements[0], 0);
                    assertInteger(elements[1], 1);
                    assertInteger(elements[2], 2);
                });
        }
    );

    // todo: error
    assertSubscriptExpr(
        "foo[]",
        [](Node const& callee) {
            assertIdentifier(callee, EToken::Identifier, "foo");
        }, [](Node const& args) {
            assertArray(args, 0);
        }
    );

    assertSubscriptExpr(
        "foo[0,1,2][bar]",
        [](Node const& callee) {
            assertSubscriptExpr(
                callee,
                [](Node const& callee1) {
                    assertIdentifier(callee1, EToken::Identifier, "foo");
                }
                ,
                [](Node const& args1) {
                    assertArray(
                        args1, 3,
                        [](tlc::Span<Node const> const elements) {
                            assertInteger(elements[0], 0);
                            assertInteger(elements[1], 1);
                            assertInteger(elements[2], 2);
                        });
                }
            );
        }, [](Node const& args) {
            assertArray(
                args, 1,
                [](tlc::Span<Node const> const elements) {
                    assertIdentifier(
                        elements[0], EToken::Identifier, "bar"
                    );
                });
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Prefix expressions", "[Parse]") {
    assertPrefixExpr(
        "!5", EToken::Exclaim,
        [](Node const& operand) {
            assertInteger(operand, 5);
        }
    );

    assertPrefixExpr(
        "!!!5", EToken::Exclaim,
        [](Node const& operand) {
            assertPrefixExpr(
                operand, EToken::Exclaim,
                [](Node const& operand1) {
                    assertPrefixExpr(
                        operand1, EToken::Exclaim,
                        [](Node const& operand2) {
                            assertInteger(operand2, 5);
                        }
                    );
                }
            );
        }
    );

    assertPrefixExpr(
        "...[0,1,2]", EToken::Dot3,
        [](Node const& operand) {
            assertArray(
                operand, 3,
                [](tlc::Span<Node const> const elements) {
                    assertInteger(elements[0], 0);
                    assertInteger(elements[1], 1);
                    assertInteger(elements[2], 2);
                }
            );
        }
    );

    assertPrefixExpr(
        "...[0,1,2]", EToken::Dot3,
        [](Node const& operand) {
            assertArray(
                operand, 3,
                [](tlc::Span<Node const> const elements) {
                    assertInteger(elements[0], 0);
                    assertInteger(elements[1], 1);
                    assertInteger(elements[2], 2);
                }
            );
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
        assertIdentifier(
            left.operand(), EToken::Identifier, "x"
        );

        assertInteger(expr.right(), 3);
    }

    SECTION("Prioritize right") {
        auto const expr =
            parseExpr<expr::Binary>("-x+y*z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Prefix>(expr.left());
        REQUIRE(left.op() == EToken::Minus);
        assertIdentifier(
            left.operand(), EToken::Identifier, "x"
        );

        auto const right =
            cast<expr::Binary>(expr.right());
        REQUIRE(right.op() == EToken::Star);
        assertIdentifier(
            right.left(), EToken::Identifier, "y"
        );
        assertIdentifier(
            right.right(), EToken::Identifier, "z"
        );
    }

    SECTION("Prioritize left") {
        auto const expr =
            parseExpr<expr::Binary>("x*-y+z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Binary>(expr.left());
        REQUIRE(left.op() == EToken::Star);
        assertIdentifier(
            left.left(), EToken::Identifier, "x"
        );
        auto const leftRight =
            cast<expr::Prefix>(left.right());
        REQUIRE(leftRight.op() == EToken::Minus);
        assertIdentifier(
            leftRight.operand(),
            EToken::Identifier, "y"
        );

        assertIdentifier(
            expr.right(), EToken::Identifier, "z"
        );
    }
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}
