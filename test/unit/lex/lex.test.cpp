#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

// todo: test cases for lexing errors

class LexTestFixture {
protected:
    auto lex(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_tokens = tlc::lex::Lex::operator()(std::move(iss));
    }

    auto assertTokenAt(
        tlc::szt const i, tlc::lexeme::Lexeme const& lexeme, tlc::StrV const str,
        tlc::szt const line, tlc::szt const column
    ) const -> void {
        CAPTURE(i);
        REQUIRE(i < m_tokens.size());
        REQUIRE(m_tokens[i].lexeme() == lexeme);
        REQUIRE(m_tokens[i].str() == str);
        REQUIRE(m_tokens[i].line() == line);
        REQUIRE(m_tokens[i].column() == column);
    }

    auto assertTokenCount(tlc::szt const count) const -> void {
        REQUIRE(count == m_tokens.size());
    }

private:
    tlc::token::TokenizedBuffer m_tokens;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Lex: Spaces correctly ignored", "[Lex]") {
    lex(R"(
foo Bar    Float

    module
    )");

    assertTokenCount(4);
    assertTokenAt(0, tlc::lexeme::identifier, "foo", 1, 0);
    assertTokenAt(1, tlc::lexeme::userDefinedType, "Bar", 1, 4);
    assertTokenAt(2, tlc::lexeme::fundamentalType, "Float", 1, 11);
    assertTokenAt(3, tlc::lexeme::module, "module", 3, 4);
}

TEST_CASE_WITH_FIXTURE("Lex: Comments", "[Lex]") {}

TEST_CASE_WITH_FIXTURE("Lex: Identifiers and keywords", "[Lex]") {
    SECTION("Fundamental types") {
        lex(R"(
Int
Float
Bool
Char
Void
String
Any
Opt
Own
Ref
Obs
        )");

        assertTokenCount(11);
        assertTokenAt(0, tlc::lexeme::fundamentalType, "Int", 1, 0);
        assertTokenAt(1, tlc::lexeme::fundamentalType, "Float", 2, 0);
        assertTokenAt(2, tlc::lexeme::fundamentalType, "Bool", 3, 0);
        assertTokenAt(3, tlc::lexeme::fundamentalType, "Char", 4, 0);
        assertTokenAt(4, tlc::lexeme::fundamentalType, "Void", 5, 0);
        assertTokenAt(5, tlc::lexeme::fundamentalType, "String", 6, 0);
        assertTokenAt(6, tlc::lexeme::fundamentalType, "Any", 7, 0);
        assertTokenAt(7, tlc::lexeme::fundamentalType, "Opt", 8, 0);
        assertTokenAt(8, tlc::lexeme::fundamentalType, "Own", 9, 0);
        assertTokenAt(9, tlc::lexeme::fundamentalType, "Ref", 10, 0);
        assertTokenAt(10, tlc::lexeme::fundamentalType, "Obs", 11, 0);
    }

    SECTION("Identifiers") {
        lex(R"(
foo
f0o
fo0
        )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::lexeme::identifier, "foo", 1, 0);
        assertTokenAt(1, tlc::lexeme::identifier, "f0o", 2, 0);
        assertTokenAt(2, tlc::lexeme::identifier, "fo0", 3, 0);
    }

    SECTION("UserDefinedType") {
        lex(R"(
Foo
F0o
Fo0
        )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::lexeme::userDefinedType, "Foo", 1, 0);
        assertTokenAt(1, tlc::lexeme::userDefinedType, "F0o", 2, 0);
        assertTokenAt(2, tlc::lexeme::userDefinedType, "Fo0", 3, 0);
    }

    SECTION("Keywords") {
        SECTION("Module") {
            lex(R"(
module
import
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::lexeme::module, "module", 1, 0);
            assertTokenAt(1, tlc::lexeme::import_, "import", 2, 0);
        }

        SECTION("Visibility") {
            lex(R"(
pub
prv
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::lexeme::pub, "pub", 1, 0);
            assertTokenAt(1, tlc::lexeme::prv, "prv", 2, 0);
        }

        SECTION("Storage") {
            lex(R"(
isolated
static
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::lexeme::isolated, "isolated", 1, 0);
            assertTokenAt(1, tlc::lexeme::static_, "static", 2, 0);
        }

        SECTION("Definition") {
            lex(R"(
let
fn
trait
type
enum
flag
            )");

            assertTokenCount(6);
            assertTokenAt(0, tlc::lexeme::let, "let", 1, 0);
            assertTokenAt(1, tlc::lexeme::fn, "fn", 2, 0);
            assertTokenAt(2, tlc::lexeme::trait, "trait", 3, 0);
            assertTokenAt(3, tlc::lexeme::type, "type", 4, 0);
            assertTokenAt(4, tlc::lexeme::enum_, "enum", 5, 0);
            assertTokenAt(5, tlc::lexeme::flag, "flag", 6, 0);
        }

        SECTION("Control") {
            lex(R"(
for
return
match
defer
preface
break
continue
            )");

            assertTokenCount(7);
            assertTokenAt(0, tlc::lexeme::for_, "for", 1, 0);
            assertTokenAt(1, tlc::lexeme::return_, "return", 2, 0);
            assertTokenAt(2, tlc::lexeme::match, "match", 3, 0);
            assertTokenAt(3, tlc::lexeme::defer, "defer", 4, 0);
            assertTokenAt(4, tlc::lexeme::preface, "preface", 5, 0);
            assertTokenAt(5, tlc::lexeme::break_, "break", 6, 0);
            assertTokenAt(6, tlc::lexeme::continue_, "continue", 7, 0);
        }

        SECTION("Adverb") {
            lex(R"(
in
when
impl
            )");

            assertTokenCount(3);
            assertTokenAt(0, tlc::lexeme::in, "in", 1, 0);
            assertTokenAt(1, tlc::lexeme::when, "when", 2, 0);
            assertTokenAt(2, tlc::lexeme::impl, "impl", 3, 0);
        }

        SECTION("Reserved") {
            lex(R"(
self
main
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::lexeme::self, "self", 1, 0);
            assertTokenAt(1, tlc::lexeme::main_, "main", 2, 0);
        }

        SECTION("Boolean") {
            lex(R"(
true
false
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::lexeme::true_, "true", 1, 0);
            assertTokenAt(1, tlc::lexeme::false_, "false", 2, 0);
        }
    }
}

TEST_CASE_WITH_FIXTURE("Lex: Numbers", "[Lex]") {
    SECTION("Valid") {
        lex(R"(
31415
0

3.1415
31.415
03.1415
0.0314
00.0314
0.0
0.00
00.00

0b10101010
0b0
0b00

0x123456789abcdef
0x0
0x00

01234567
00
000
        )");

        assertTokenCount(19);

        assertTokenAt(
            0, tlc::lexeme::integer10Literal, "31415",
            1, 0
        );
        assertTokenAt(
            1, tlc::lexeme::integer10Literal, "0",
            2, 0
        );

        assertTokenAt(
            2, tlc::lexeme::floatLiteral, "3.1415",
            4, 0);
        assertTokenAt(
            3, tlc::lexeme::floatLiteral,
            "31.415", 5, 0
        );
        assertTokenAt(
            4, tlc::lexeme::floatLiteral,
            "03.1415", 6, 0
        );
        assertTokenAt(
            5, tlc::lexeme::floatLiteral,
            "0.0314", 7, 0
        );
        assertTokenAt(
            6, tlc::lexeme::floatLiteral,
            "00.0314", 8, 0
        );
        assertTokenAt(
            7, tlc::lexeme::floatLiteral,
            "0.0", 9, 0
        );
        assertTokenAt(
            8, tlc::lexeme::floatLiteral,
            "0.00", 10, 0
        );
        assertTokenAt(
            9, tlc::lexeme::floatLiteral,
            "00.00", 11, 0
        );

        assertTokenAt(
            10, tlc::lexeme::integer2Literal,
            "0b10101010", 13, 0
        );
        assertTokenAt(
            11, tlc::lexeme::integer2Literal,
            "0b0", 14, 0
        );
        assertTokenAt(
            12, tlc::lexeme::integer2Literal,
            "0b00", 15, 0
        );

        assertTokenAt(
            13, tlc::lexeme::integer16Literal,
            "0x123456789abcdef", 17, 0
        );
        assertTokenAt(
            14, tlc::lexeme::integer16Literal,
            "0x0", 18, 0
        );
        assertTokenAt(
            15, tlc::lexeme::integer16Literal,
            "0x00", 19, 0
        );

        assertTokenAt(
            16, tlc::lexeme::integer8Literal,
            "01234567", 21, 0
        );
        assertTokenAt(
            17, tlc::lexeme::integer8Literal,
            "00", 22, 0
        );
        assertTokenAt(
            18, tlc::lexeme::integer8Literal,
            "000", 23, 0
        );
    }
}

TEST_CASE_WITH_FIXTURE("Lex: Symbols", "[Lex]") {
    SECTION("Single character") {
        lex(R"(
(
)
[
]
{
}
.
,
:
;
*
&
#
|
^
+
-
/
%
!
=
>
<
?
~
$
@
        )");

        assertTokenCount(27);
        assertTokenAt(0, tlc::lexeme::leftParen, "(", 1, 0);
        assertTokenAt(1, tlc::lexeme::rightParen, ")", 2, 0);
        assertTokenAt(2, tlc::lexeme::leftBracket, "[", 3, 0);
        assertTokenAt(3, tlc::lexeme::rightBracket, "]", 4, 0);
        assertTokenAt(4, tlc::lexeme::leftBrace, "{", 5, 0);
        assertTokenAt(5, tlc::lexeme::rightBrace, "}", 6, 0);
        assertTokenAt(6, tlc::lexeme::dot, ".", 7, 0);
        assertTokenAt(7, tlc::lexeme::comma, ",", 8, 0);
        assertTokenAt(8, tlc::lexeme::colon, ":", 9, 0);
        assertTokenAt(9, tlc::lexeme::semicolon, ";", 10, 0);
        assertTokenAt(10, tlc::lexeme::star, "*", 11, 0);
        assertTokenAt(11, tlc::lexeme::ampersand, "&", 12, 0);
        assertTokenAt(12, tlc::lexeme::hash, "#", 13, 0);
        assertTokenAt(13, tlc::lexeme::bar, "|", 14, 0);
        assertTokenAt(14, tlc::lexeme::hat, "^", 15, 0);
        assertTokenAt(15, tlc::lexeme::plus, "+", 16, 0);
        assertTokenAt(16, tlc::lexeme::minus, "-", 17, 0);
        assertTokenAt(17, tlc::lexeme::fwdSlash, "/", 18, 0);
        assertTokenAt(18, tlc::lexeme::percent, "%", 19, 0);
        assertTokenAt(19, tlc::lexeme::exclaim, "!", 20, 0);
        assertTokenAt(20, tlc::lexeme::equal, "=", 21, 0);
        assertTokenAt(21, tlc::lexeme::greater, ">", 22, 0);
        assertTokenAt(22, tlc::lexeme::less, "<", 23, 0);
        assertTokenAt(23, tlc::lexeme::qMark, "?", 24, 0);
        assertTokenAt(24, tlc::lexeme::tilde, "~", 25, 0);
        assertTokenAt(25, tlc::lexeme::dollar, "$", 26, 0);
        assertTokenAt(26, tlc::lexeme::at, "@", 27, 0);
    }

    SECTION("Double characters") {
        lex(R"(
!=
*=
&=
|=
^=
/=
%=
>=
<=
+=
-=
|>
->
=>
::
**
&&
||
++
--
==
>>
<<
??
..
        )");

        assertTokenCount(25);
        assertTokenAt(0, tlc::lexeme::exclaimEqual, "!=", 1, 0);
        assertTokenAt(1, tlc::lexeme::starEqual, "*=", 2, 0);
        assertTokenAt(2, tlc::lexeme::ampersandEqual, "&=", 3, 0);
        assertTokenAt(3, tlc::lexeme::barEqual, "|=", 4, 0);
        assertTokenAt(4, tlc::lexeme::hatEqual, "^=", 5, 0);
        assertTokenAt(5, tlc::lexeme::fwdSlashEqual, "/=", 6, 0);
        assertTokenAt(6, tlc::lexeme::percentEqual, "%=", 7, 0);
        assertTokenAt(7, tlc::lexeme::greaterEqual, ">=", 8, 0);
        assertTokenAt(8, tlc::lexeme::lessEqual, "<=", 9, 0);
        assertTokenAt(9, tlc::lexeme::plusEqual, "+=", 10, 0);
        assertTokenAt(10, tlc::lexeme::minusEqual, "-=", 11, 0);
        assertTokenAt(11, tlc::lexeme::barGreater, "|>", 12, 0);
        assertTokenAt(12, tlc::lexeme::minusGreater, "->", 13, 0);
        assertTokenAt(13, tlc::lexeme::equalGreater, "=>", 14, 0);
        assertTokenAt(14, tlc::lexeme::colon2, "::", 15, 0);
        assertTokenAt(15, tlc::lexeme::star2, "**", 16, 0);
        assertTokenAt(16, tlc::lexeme::ampersand2, "&&", 17, 0);
        assertTokenAt(17, tlc::lexeme::bar2, "||", 18, 0);
        assertTokenAt(18, tlc::lexeme::plus2, "++", 19, 0);
        assertTokenAt(19, tlc::lexeme::minus2, "--", 20, 0);
        assertTokenAt(20, tlc::lexeme::equal2, "==", 21, 0);
        assertTokenAt(21, tlc::lexeme::greater2, ">>", 22, 0);
        assertTokenAt(22, tlc::lexeme::less2, "<<", 23, 0);
        assertTokenAt(23, tlc::lexeme::qMark2, "??", 24, 0);
        assertTokenAt(24, tlc::lexeme::dot2, "..", 25, 0);
    }

    SECTION("Triple characters") {
        lex(R"(
>>=
<<=
**=
...
        )");

        assertTokenCount(4);
        assertTokenAt(0, tlc::lexeme::greater2Equal, ">>=", 1, 0);
        assertTokenAt(1, tlc::lexeme::less2Equal, "<<=", 2, 0);
        assertTokenAt(2, tlc::lexeme::star2Equal, "**=", 3, 0);
        assertTokenAt(3, tlc::lexeme::dot3, "...", 4, 0);
    }
}

TEST_CASE_WITH_FIXTURE("Lex: Strings", "[Lex]") {
    // todo: \n, \r
    SECTION("Literals") {
        lex(R"(
""
"hello, world!\n"
"\{ \}"
"\t \\ \b \a \f"
        )");

        // \n \t \r \b \f \a \\ \' \"

        assertTokenCount(4);
        assertTokenAt(0, tlc::lexeme::stringFragment, "", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringFragment, "hello, world!\n", 2, 0);
        assertTokenAt(2, tlc::lexeme::stringFragment, "{ }", 3, 0);
        assertTokenAt(3, tlc::lexeme::stringFragment, "\t \\ \b \a \f", 4, 0);
    }

    SECTION("Only placeholder") {
        lex(R"(
"{}"
                )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::lexeme::stringFragment, "", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringPlaceholder, "", 1, 1);
        assertTokenAt(2, tlc::lexeme::stringFragment, "", 1, 3);
    }

    SECTION("Placeholder at the beginning") {
        lex(R"(
"{x}{5}text"
                )");

        assertTokenCount(5);
        assertTokenAt(0, tlc::lexeme::stringFragment, "", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringPlaceholder, "x", 1, 1);
        assertTokenAt(2, tlc::lexeme::stringFragment, "", 1, 4);
        assertTokenAt(3, tlc::lexeme::stringPlaceholder, "5", 1, 4);
        assertTokenAt(4, tlc::lexeme::stringFragment, "text", 1, 7);
    }

    SECTION("Placeholder at the end") {
        lex(R"(
"text{""}{"inner"}"
                    )");

        assertTokenCount(5);
        assertTokenAt(0, tlc::lexeme::stringFragment, "text", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringPlaceholder, "\"\"", 1, 5);
        assertTokenAt(2, tlc::lexeme::stringFragment, "", 1, 9);
        assertTokenAt(3, tlc::lexeme::stringPlaceholder, "\"inner\"", 1, 9);
        assertTokenAt(4, tlc::lexeme::stringFragment, "", 1, 18);
    }

    SECTION("Placeholder in the middle") {
        lex(R"(
"left{}right"
                    )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::lexeme::stringFragment, "left", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringPlaceholder, "", 1, 5);
        assertTokenAt(2, tlc::lexeme::stringFragment, "right", 1, 7);
    }

    SECTION("{x}+{y}={x+y}") {
        lex(R"(
"{x}+{y}={x+y}"
                    )");

        assertTokenCount(7);
        assertTokenAt(0, tlc::lexeme::stringFragment, "", 1, 0);
        assertTokenAt(1, tlc::lexeme::stringPlaceholder, "x", 1, 1);
        assertTokenAt(2, tlc::lexeme::stringFragment, "+", 1, 4);
        assertTokenAt(3, tlc::lexeme::stringPlaceholder, "y", 1, 5);
        assertTokenAt(4, tlc::lexeme::stringFragment, "=", 1, 8);
        assertTokenAt(5, tlc::lexeme::stringPlaceholder, "x+y", 1, 9);
        assertTokenAt(6, tlc::lexeme::stringFragment, "", 1, 14);
    }
}
