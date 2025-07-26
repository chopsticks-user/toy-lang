#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

auto ParseTestFixture::assertIdentifier(
    Node const& node, tlc::Str const& path
) -> void {
    auto const identifier = cast<expr::Identifier>(node);
    REQUIRE(identifier.path() == path);
}

auto ParseTestFixture::assertIdentifier(
    tlc::Str source, tlc::Str const& path
) -> void {
    return assertIdentifier(
        parseExpr<expr::Identifier>(std::move(source)), path
    );
}

auto ParseTestFixture::assertInteger(
    Node const& node, tlc::i64 const value
) -> void {
    auto const integer = cast<expr::Integer>(node);
    REQUIRE(integer.value() == value);
}

auto ParseTestFixture::assertInteger(
    tlc::Str source, tlc::i64 const value
) -> void {
    return assertInteger(
        parseExpr<expr::Integer>(std::move(source)), value
    );
}

auto ParseTestFixture::assertFloat(
    Node const& node, tlc::f64 const value
) -> void {
    auto const f = cast<expr::Float>(node);
    REQUIRE(f.value() == value);
}

auto ParseTestFixture::assertFloat(
    tlc::Str source, tlc::f64 const value
) -> void {
    return assertFloat(
        parseExpr<expr::Float>(std::move(source)), value
    );
}

auto ParseTestFixture::assertBoolean(
    Node const& node, tlc::f64 const value
) -> void {
    auto const f = cast<expr::Boolean>(node);
    REQUIRE(f.value() == value);
}

auto ParseTestFixture::assertBoolean(
    tlc::Str source, tlc::b8 const value
) -> void {
    return assertBoolean(
        parseExpr<expr::Boolean>(std::move(source)), value
    );
}

auto ParseTestFixture::assertTuple(
    Node const& node, tlc::szt const size,
    tlc::Opt<FnNodes> const fn
) -> void {
    auto const tuple = cast<expr::Tuple>(node);
    REQUIRE(tuple.nChildren() == size);
    if (fn) {
        fn.value()(tuple.children());
    }
}

auto ParseTestFixture::assertTuple(
    tlc::Str source, tlc::szt const size,
    tlc::Opt<FnNodes> fn
) -> void {
    return assertTuple(
        parseExpr<expr::Tuple>(std::move(source)),
        size, std::move(fn)
    );
}

auto ParseTestFixture::assertArray(
    Node const& node, tlc::szt const size,
    tlc::Opt<FnNodes> const fn
) -> void {
    auto const array = cast<expr::Array>(node);
    REQUIRE(array.nChildren() == size);
    if (fn) {
        fn.value()(array.children());
    }
}

auto ParseTestFixture::assertArray(
    tlc::Str source, tlc::szt const size,
    tlc::Opt<FnNodes> fn
) -> void {
    return assertArray(
        parseExpr<expr::Array>(std::move(source)),
        size, std::move(fn)
    );
}

auto ParseTestFixture::assertAccessExpr(
    Node const& node, tlc::Opt<FnNode> const fnObj,
    tlc::Opt<FnNode> const fnField
) -> void {
    auto const expr = cast<expr::Access>(node);
    if (fnObj) {
        fnObj.value()(expr.object());
    }
    if (fnField) {
        fnField.value()(expr.field());
    }
}

auto ParseTestFixture::assertAccessExpr(
    tlc::Str source, tlc::Opt<FnNode> const fnObj,
    tlc::Opt<FnNode> fnField
) -> void {
    return assertAccessExpr(
        parseExpr<expr::Access>(std::move(source)),
        std::move(fnObj), std::move(fnField)
    );
}

auto ParseTestFixture::assertFnAppExpr(
    Node const& node, tlc::Opt<FnNode> const fnCallee,
    tlc::Opt<FnNode> const fnArgs
) -> void {
    auto const expr = cast<expr::FnApp>(node);
    if (fnCallee) {
        fnCallee.value()(expr.callee());
    }
    if (fnArgs) {
        fnArgs.value()(expr.args());
    }
}

auto ParseTestFixture::assertFnAppExpr(
    tlc::Str source, tlc::Opt<FnNode> const fnCallee,
    tlc::Opt<FnNode> fnArgs
) -> void {
    return assertFnAppExpr(
        parseExpr<expr::FnApp>(std::move(source)),
        std::move(fnCallee), std::move(fnArgs)
    );
}

auto ParseTestFixture::assertSubscriptExpr(
    Node const& node, tlc::Opt<FnNode> const fnColl,
    tlc::Opt<FnNode> const fnSubs
) -> void {
    auto const expr = cast<expr::Subscript>(node);
    if (fnColl) {
        fnColl.value()(expr.collection());
    }
    if (fnSubs) {
        fnSubs.value()(expr.subscript());
    }
}

auto ParseTestFixture::assertSubscriptExpr(
    tlc::Str source, tlc::Opt<FnNode> const fnColl,
    tlc::Opt<FnNode> fnSubs
) -> void {
    return assertSubscriptExpr(
        parseExpr<expr::Subscript>(std::move(source)),
        std::move(fnColl),
        std::move(fnSubs)
    );
}

auto ParseTestFixture::assertPrefixExpr(
    Node const& node, EToken op,
    tlc::Opt<FnNode> const fnOperand
) -> void {
    auto const expr = cast<expr::Prefix>(node);
    REQUIRE(expr.op() == op);
    if (fnOperand) {
        fnOperand.value()(expr.operand());
    }
}

auto ParseTestFixture::assertPrefixExpr(
    tlc::Str source, EToken const op,
    tlc::Opt<FnNode> const fnOperand
) -> void {
    return assertPrefixExpr(
        parseExpr<expr::Prefix>(std::move(source)),
        op, std::move(fnOperand)
    );
}

// todo: assertBinaryExpr

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

// todo
TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    SECTION("Local identifier") {
        assertIdentifier("baz", "baz");
    }

    SECTION("Imported identifier") {
        assertIdentifier("foo::bar", "foo::bar");
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Tuples", "[Parse]") {
    assertTuple(
        "(1,x,2.1)", 3, [](tlc::Span<Node const> const elements) {
            assertInteger(elements[0], 1);
            assertIdentifier(elements[1], "x");
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
            assertIdentifier(elements[1], "x");
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
            assertIdentifier(object, "foo");
        },
        [](Node const& field) {
            assertIdentifier(field, "bar");
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
            assertIdentifier(field, "foo");
        }
    );

    assertAccessExpr(
        "foo.bar.baz",
        [](Node const& object) {
            assertAccessExpr(
                object,
                [](Node const& object1) {
                    assertIdentifier(object1, "foo");
                },
                [](Node const& field1) {
                    assertIdentifier(field1, "bar");
                }
            );
        },
        [](Node const& field) {
            assertIdentifier(field, "baz");
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Parse]") {
    assertFnAppExpr(
        "foo(0,1,2)",
        [](Node const& callee) {
            assertIdentifier(callee, "foo");
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
            assertIdentifier(callee, "foo");
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
                    assertIdentifier(callee1, "foo");
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
                        elements[0], "bar"
                    );
                });
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Parse]") {
    assertSubscriptExpr(
        "foo[0,1,2]",
        [](Node const& callee) {
            assertIdentifier(callee, "foo");
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
            assertIdentifier(callee, "foo");
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
                    assertIdentifier(callee1, "foo");
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
                        elements[0], "bar"
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
            left.operand(), "x"
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
            left.operand(), "x"
        );

        auto const right =
            cast<expr::Binary>(expr.right());
        REQUIRE(right.op() == EToken::Star);
        assertIdentifier(
            right.left(), "y"
        );
        assertIdentifier(
            right.right(), "z"
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
            left.left(), "x"
        );
        auto const leftRight =
            cast<expr::Prefix>(left.right());
        REQUIRE(leftRight.op() == EToken::Minus);
        assertIdentifier(
            leftRight.operand(), "y"
        );

        assertIdentifier(
            expr.right(), "z"
        );
    }
}

// todo
TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}
