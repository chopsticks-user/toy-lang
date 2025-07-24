#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

// todo: test cases for lexing errors

class LexTestFixture {
protected:
    auto lex(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_tokens = tlc::lex::Lexer::operator()(std::move(iss));
    }

    auto assertTokenAt(
        tlc::szt const i, tlc::token::EToken const type, tlc::StrV const str,
        tlc::szt const line, tlc::szt const column
    ) const -> void {
        CAPTURE(i);
        REQUIRE(i < m_tokens.size());
        REQUIRE(m_tokens[i].type() == type);
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
int
    module
    )");

    assertTokenCount(4);
    assertTokenAt(0, tlc::token::EToken::Identifier, "foo", 1, 0);
    assertTokenAt(1, tlc::token::EToken::UserDefinedType, "Bar", 1, 4);
    assertTokenAt(2, tlc::token::EToken::FundamentalType, "Float", 1, 11);
    // "int" is a reserved keyword
    assertTokenAt(3, tlc::token::EToken::Module, "module", 3, 4);
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
        )");

        assertTokenCount(8);
        assertTokenAt(0, tlc::token::EToken::FundamentalType, "Int", 1, 0);
        assertTokenAt(1, tlc::token::EToken::FundamentalType, "Float", 2, 0);
        assertTokenAt(2, tlc::token::EToken::FundamentalType, "Bool", 3, 0);
        assertTokenAt(3, tlc::token::EToken::FundamentalType, "Char", 4, 0);
        assertTokenAt(4, tlc::token::EToken::FundamentalType, "Void", 5, 0);
        assertTokenAt(5, tlc::token::EToken::FundamentalType, "String", 6, 0);
        assertTokenAt(6, tlc::token::EToken::FundamentalType, "Any", 7, 0);
        assertTokenAt(7, tlc::token::EToken::FundamentalType, "Opt", 8, 0);
    }

    SECTION("Reserved keywords") {
        lex(R"(
int
float
bool
char
void
string
any
opt
        )");

        assertTokenCount(0);
    }

    SECTION("Identifiers") {
        lex(R"(
foo
f0o
fo0
        )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::token::EToken::Identifier, "foo", 1, 0);
        assertTokenAt(1, tlc::token::EToken::Identifier, "f0o", 2, 0);
        assertTokenAt(2, tlc::token::EToken::Identifier, "fo0", 3, 0);
    }

    SECTION("UserDefinedType") {
        lex(R"(
Foo
F0o
Fo0
        )");

        assertTokenCount(3);
        assertTokenAt(0, tlc::token::EToken::UserDefinedType, "Foo", 1, 0);
        assertTokenAt(1, tlc::token::EToken::UserDefinedType, "F0o", 2, 0);
        assertTokenAt(2, tlc::token::EToken::UserDefinedType, "Fo0", 3, 0);
    }

    SECTION("Keywords") {
        SECTION("Visibility") {
            lex(R"(
module
import
export
internal
local
extern
            )");

            assertTokenCount(6);
            assertTokenAt(0, tlc::token::EToken::Module, "module", 1, 0);
            assertTokenAt(1, tlc::token::EToken::Import, "import", 2, 0);
            assertTokenAt(2, tlc::token::EToken::Export, "export", 3, 0);
            assertTokenAt(3, tlc::token::EToken::Internal, "internal", 4, 0);
            assertTokenAt(4, tlc::token::EToken::Local, "local", 5, 0);
            assertTokenAt(5, tlc::token::EToken::Extern, "extern", 6, 0);
        }

        SECTION("Definition") {
            lex(R"(
let
mut
fn
trait
type
enum
flag
            )");

            assertTokenCount(7);
            assertTokenAt(0, tlc::token::EToken::Let, "let", 1, 0);
            assertTokenAt(1, tlc::token::EToken::Mut, "mut", 2, 0);
            assertTokenAt(2, tlc::token::EToken::Fn, "fn", 3, 0);
            assertTokenAt(3, tlc::token::EToken::Trait, "trait", 4, 0);
            assertTokenAt(4, tlc::token::EToken::Type, "type", 5, 0);
            assertTokenAt(5, tlc::token::EToken::Enum, "enum", 6, 0);
            assertTokenAt(6, tlc::token::EToken::Flag, "flag", 7, 0);
        }

        SECTION("Control") {
            lex(R"(
for
return
match
            )");

            assertTokenCount(3);
            assertTokenAt(0, tlc::token::EToken::For, "for", 1, 0);
            assertTokenAt(1, tlc::token::EToken::Return, "return", 2, 0);
            assertTokenAt(2, tlc::token::EToken::Match, "match", 3, 0);
        }

        SECTION("Adverb") {
            lex(R"(
by
of
in
when
            )");

            assertTokenCount(4);
            assertTokenAt(0, tlc::token::EToken::By, "by", 1, 0);
            assertTokenAt(1, tlc::token::EToken::Of, "of", 2, 0);
            assertTokenAt(2, tlc::token::EToken::In, "in", 3, 0);
            assertTokenAt(3, tlc::token::EToken::When, "when", 4, 0);
        }

        SECTION("Boolean") {
            lex(R"(
true
false
            )");

            assertTokenCount(2);
            assertTokenAt(0, tlc::token::EToken::True, "true", 1, 0);
            assertTokenAt(1, tlc::token::EToken::False, "false", 2, 0);
        }

        SECTION("Object") {
            lex(R"(
self
pub
prv
impl
            )");

            assertTokenCount(4);
            assertTokenAt(0, tlc::token::EToken::Self, "self", 1, 0);
            assertTokenAt(1, tlc::token::EToken::Pub, "pub", 2, 0);
            assertTokenAt(2, tlc::token::EToken::Prv, "prv", 3, 0);
            assertTokenAt(3, tlc::token::EToken::Impl, "impl", 4, 0);
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
            0, tlc::token::EToken::Integer10Literal, "31415",
            1, 0
        );
        assertTokenAt(
            1, tlc::token::EToken::Integer10Literal, "0",
            2, 0
        );

        assertTokenAt(
            2, tlc::token::EToken::FloatLiteral, "3.1415",
            4, 0);
        assertTokenAt(
            3, tlc::token::EToken::FloatLiteral,
            "31.415", 5, 0
        );
        assertTokenAt(
            4, tlc::token::EToken::FloatLiteral,
            "03.1415", 6, 0
        );
        assertTokenAt(
            5, tlc::token::EToken::FloatLiteral,
            "0.0314", 7, 0
        );
        assertTokenAt(
            6, tlc::token::EToken::FloatLiteral,
            "00.0314", 8, 0
        );
        assertTokenAt(
            7, tlc::token::EToken::FloatLiteral,
            "0.0", 9, 0
        );
        assertTokenAt(
            8, tlc::token::EToken::FloatLiteral,
            "0.00", 10, 0
        );
        assertTokenAt(
            9, tlc::token::EToken::FloatLiteral,
            "00.00", 11, 0
        );

        assertTokenAt(
            10, tlc::token::EToken::Integer2Literal,
            "0b10101010", 13, 0
        );
        assertTokenAt(
            11, tlc::token::EToken::Integer2Literal,
            "0b0", 14, 0
        );
        assertTokenAt(
            12, tlc::token::EToken::Integer2Literal,
            "0b00", 15, 0
        );

        assertTokenAt(
            13, tlc::token::EToken::Integer16Literal,
            "0x123456789abcdef", 17, 0
        );
        assertTokenAt(
            14, tlc::token::EToken::Integer16Literal,
            "0x0", 18, 0
        );
        assertTokenAt(
            15, tlc::token::EToken::Integer16Literal,
            "0x00", 19, 0
        );

        assertTokenAt(
            16, tlc::token::EToken::Integer8Literal,
            "01234567", 21, 0
        );
        assertTokenAt(
            17, tlc::token::EToken::Integer8Literal,
            "00", 22, 0
        );
        assertTokenAt(
            18, tlc::token::EToken::Integer8Literal,
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
'
"
?
~
$
@
        )");

        assertTokenCount(29);
        assertTokenAt(0, tlc::token::EToken::LeftParen, "(", 1, 0);
        assertTokenAt(1, tlc::token::EToken::RightParen, ")", 2, 0);
        assertTokenAt(2, tlc::token::EToken::LeftBracket, "[", 3, 0);
        assertTokenAt(3, tlc::token::EToken::RightBracket, "]", 4, 0);
        assertTokenAt(4, tlc::token::EToken::LeftBrace, "{", 5, 0);
        assertTokenAt(5, tlc::token::EToken::RightBrace, "}", 6, 0);
        assertTokenAt(6, tlc::token::EToken::Dot, ".", 7, 0);
        assertTokenAt(7, tlc::token::EToken::Comma, ",", 8, 0);
        assertTokenAt(8, tlc::token::EToken::Colon, ":", 9, 0);
        assertTokenAt(9, tlc::token::EToken::Semicolon, ";", 10, 0);
        assertTokenAt(10, tlc::token::EToken::Star, "*", 11, 0);
        assertTokenAt(11, tlc::token::EToken::Ampersand, "&", 12, 0);
        assertTokenAt(12, tlc::token::EToken::Hash, "#", 13, 0);
        assertTokenAt(13, tlc::token::EToken::Bar, "|", 14, 0);
        assertTokenAt(14, tlc::token::EToken::Hat, "^", 15, 0);
        assertTokenAt(15, tlc::token::EToken::Plus, "+", 16, 0);
        assertTokenAt(16, tlc::token::EToken::Minus, "-", 17, 0);
        assertTokenAt(17, tlc::token::EToken::FwdSlash, "/", 18, 0);
        assertTokenAt(18, tlc::token::EToken::Percent, "%", 19, 0);
        assertTokenAt(19, tlc::token::EToken::Exclaim, "!", 20, 0);
        assertTokenAt(20, tlc::token::EToken::Equal, "=", 21, 0);
        assertTokenAt(21, tlc::token::EToken::Greater, ">", 22, 0);
        assertTokenAt(22, tlc::token::EToken::Less, "<", 23, 0);
        assertTokenAt(23, tlc::token::EToken::SQuote, "'", 24, 0);
        assertTokenAt(24, tlc::token::EToken::DQuote, "\"", 25, 0);
        assertTokenAt(25, tlc::token::EToken::QMark, "?", 26, 0);
        assertTokenAt(26, tlc::token::EToken::Tilde, "~", 27, 0);
        assertTokenAt(27, tlc::token::EToken::Dollar, "$", 28, 0);
        assertTokenAt(28, tlc::token::EToken::At, "@", 29, 0);
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
        assertTokenAt(0, tlc::token::EToken::ExclaimEqual, "!=", 1, 0);
        assertTokenAt(1, tlc::token::EToken::StarEqual, "*=", 2, 0);
        assertTokenAt(2, tlc::token::EToken::AmpersandEqual, "&=", 3, 0);
        assertTokenAt(3, tlc::token::EToken::BarEqual, "|=", 4, 0);
        assertTokenAt(4, tlc::token::EToken::HatEqual, "^=", 5, 0);
        assertTokenAt(5, tlc::token::EToken::FwdSlashEqual, "/=", 6, 0);
        assertTokenAt(6, tlc::token::EToken::PercentEqual, "%=", 7, 0);
        assertTokenAt(7, tlc::token::EToken::GreaterEqual, ">=", 8, 0);
        assertTokenAt(8, tlc::token::EToken::LessEqual, "<=", 9, 0);
        assertTokenAt(9, tlc::token::EToken::PlusEqual, "+=", 10, 0);
        assertTokenAt(10, tlc::token::EToken::MinusEqual, "-=", 11, 0);
        assertTokenAt(11, tlc::token::EToken::BarGreater, "|>", 12, 0);
        assertTokenAt(12, tlc::token::EToken::MinusGreater, "->", 13, 0);
        assertTokenAt(13, tlc::token::EToken::EqualGreater, "=>", 14, 0);
        assertTokenAt(14, tlc::token::EToken::Colon2, "::", 15, 0);
        assertTokenAt(15, tlc::token::EToken::Star2, "**", 16, 0);
        assertTokenAt(16, tlc::token::EToken::Ampersand2, "&&", 17, 0);
        assertTokenAt(17, tlc::token::EToken::Bar2, "||", 18, 0);
        assertTokenAt(18, tlc::token::EToken::Plus2, "++", 19, 0);
        assertTokenAt(19, tlc::token::EToken::Minus2, "--", 20, 0);
        assertTokenAt(20, tlc::token::EToken::Equal2, "==", 21, 0);
        assertTokenAt(21, tlc::token::EToken::Greater2, ">>", 22, 0);
        assertTokenAt(22, tlc::token::EToken::Less2, "<<", 23, 0);
        assertTokenAt(23, tlc::token::EToken::QMark2, "??", 24, 0);
        assertTokenAt(24, tlc::token::EToken::Dot2, "..", 25, 0);
    }

    SECTION("Triple characters") {
        lex(R"(
:=>
:~>
>>=
<<=
**=
...
        )");

        assertTokenCount(6);
        assertTokenAt(0, tlc::token::EToken::ColonEqualGreater, ":=>", 1, 0);
        assertTokenAt(1, tlc::token::EToken::ColonTildeGreater, ":~>", 2, 0);
        assertTokenAt(2, tlc::token::EToken::Greater2Equal, ">>=", 3, 0);
        assertTokenAt(3, tlc::token::EToken::Less2Equal, "<<=", 4, 0);
        assertTokenAt(4, tlc::token::EToken::Star2Equal, "**=", 5, 0);
        assertTokenAt(5, tlc::token::EToken::Dot3, "...", 6, 0);
    }
}
