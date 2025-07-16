#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

using tlc::lex::Lexer;
using tlc::Str;
using tlc::StrV;
using tlc::Vec;
using tlc::szt;
using tlc::token::Token;
using tlc::token::EToken;

class LexTestFixture {
protected:
    auto lex(Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_tokens = tlc::apply<Lexer>(std::move(iss));
    }

    auto assertTokenAt(
        szt const i, EToken const type, StrV const str,
        szt const line, szt const column
    ) const -> void {
        CAPTURE(i);
        REQUIRE(i < m_tokens.size());
        REQUIRE(m_tokens[i].type() == type);
        REQUIRE(m_tokens[i].str() == str);
        REQUIRE(m_tokens[i].line() == line);
        REQUIRE(m_tokens[i].column() == column);
    }

    auto assertTokenCount(szt const count) const -> void {
        REQUIRE(count == m_tokens.size());
    }

private:
    Vec<Token> m_tokens;
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
    assertTokenAt(0, EToken::Identifier, "foo", 1, 0);
    assertTokenAt(1, EToken::UserDefinedType, "Bar", 1, 4);
    assertTokenAt(2, EToken::FundamentalType, "Float", 1, 11);
    // "int" is a reserved keyword
    assertTokenAt(3, EToken::Module, "module", 3, 4);
}

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
        assertTokenAt(0, EToken::FundamentalType, "Int", 1, 0);
        assertTokenAt(1, EToken::FundamentalType, "Float", 2, 0);
        assertTokenAt(2, EToken::FundamentalType, "Bool", 3, 0);
        assertTokenAt(3, EToken::FundamentalType, "Char", 4, 0);
        assertTokenAt(4, EToken::FundamentalType, "Void", 5, 0);
        assertTokenAt(5, EToken::FundamentalType, "String", 6, 0);
        assertTokenAt(6, EToken::FundamentalType, "Any", 7, 0);
        assertTokenAt(7, EToken::FundamentalType, "Opt", 8, 0);
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
        assertTokenAt(0, EToken::Identifier, "foo", 1, 0);
        assertTokenAt(1, EToken::Identifier, "f0o", 2, 0);
        assertTokenAt(2, EToken::Identifier, "fo0", 3, 0);
    }

    SECTION("UserDefinedType") {
        lex(R"(
Foo
F0o
Fo0
        )");

        assertTokenCount(3);
        assertTokenAt(0, EToken::UserDefinedType, "Foo", 1, 0);
        assertTokenAt(1, EToken::UserDefinedType, "F0o", 2, 0);
        assertTokenAt(2, EToken::UserDefinedType, "Fo0", 3, 0);
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
            assertTokenAt(0, EToken::Module, "module", 1, 0);
            assertTokenAt(1, EToken::Import, "import", 2, 0);
            assertTokenAt(2, EToken::Export, "export", 3, 0);
            assertTokenAt(3, EToken::Internal, "internal", 4, 0);
            assertTokenAt(4, EToken::Local, "local", 5, 0);
            assertTokenAt(5, EToken::Extern, "extern", 6, 0);
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
            assertTokenAt(0, EToken::Let, "let", 1, 0);
            assertTokenAt(1, EToken::Mut, "mut", 2, 0);
            assertTokenAt(2, EToken::Fn, "fn", 3, 0);
            assertTokenAt(3, EToken::Trait, "trait", 4, 0);
            assertTokenAt(4, EToken::Type, "type", 5, 0);
            assertTokenAt(5, EToken::Enum, "enum", 6, 0);
            assertTokenAt(6, EToken::Flag, "flag", 7, 0);
        }

        SECTION("Control") {
            lex(R"(
for
return
match
            )");

            assertTokenCount(3);
            assertTokenAt(0, EToken::For, "for", 1, 0);
            assertTokenAt(1, EToken::Return, "return", 2, 0);
            assertTokenAt(2, EToken::Match, "match", 3, 0);
        }

        SECTION("Adverb") {
            lex(R"(
by
of
in
when
            )");

            assertTokenCount(4);
            assertTokenAt(0, EToken::By, "by", 1, 0);
            assertTokenAt(1, EToken::Of, "of", 2, 0);
            assertTokenAt(2, EToken::In, "in", 3, 0);
            assertTokenAt(3, EToken::When, "when", 4, 0);
        }

        SECTION("Boolean") {
            lex(R"(
true
false
            )");

            assertTokenCount(2);
            assertTokenAt(0, EToken::True, "true", 1, 0);
            assertTokenAt(1, EToken::False, "false", 2, 0);
        }

        SECTION("Object") {
            lex(R"(
self
pub
prv
impl
            )");

            assertTokenCount(4);
            assertTokenAt(0, EToken::Self, "self", 1, 0);
            assertTokenAt(1, EToken::Pub, "pub", 2, 0);
            assertTokenAt(2, EToken::Prv, "prv", 3, 0);
            assertTokenAt(3, EToken::Impl, "impl", 4, 0);
        }
    }
}

TEST_CASE_WITH_FIXTURE("Lex: Numbers", "[Lex]") {}

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
        assertTokenAt(0, EToken::LeftParen, "(", 1, 0);
        assertTokenAt(1, EToken::RightParen, ")", 2, 0);
        assertTokenAt(2, EToken::LeftBracket, "[", 3, 0);
        assertTokenAt(3, EToken::RightBracket, "]", 4, 0);
        assertTokenAt(4, EToken::LeftBrace, "{", 5, 0);
        assertTokenAt(5, EToken::RightBrace, "}", 6, 0);
        assertTokenAt(6, EToken::Dot, ".", 7, 0);
        assertTokenAt(7, EToken::Comma, ",", 8, 0);
        assertTokenAt(8, EToken::Colon, ":", 9, 0);
        assertTokenAt(9, EToken::Semicolon, ";", 10, 0);
        assertTokenAt(10, EToken::Star, "*", 11, 0);
        assertTokenAt(11, EToken::Ampersand, "&", 12, 0);
        assertTokenAt(12, EToken::Hash, "#", 13, 0);
        assertTokenAt(13, EToken::Bar, "|", 14, 0);
        assertTokenAt(14, EToken::Hat, "^", 15, 0);
        assertTokenAt(15, EToken::Plus, "+", 16, 0);
        assertTokenAt(16, EToken::Minus, "-", 17, 0);
        assertTokenAt(17, EToken::FwdSlash, "/", 18, 0);
        assertTokenAt(18, EToken::Percent, "%", 19, 0);
        assertTokenAt(19, EToken::Exclaim, "!", 20, 0);
        assertTokenAt(20, EToken::Equal, "=", 21, 0);
        assertTokenAt(21, EToken::Greater, ">", 22, 0);
        assertTokenAt(22, EToken::Less, "<", 23, 0);
        assertTokenAt(23, EToken::SQuote, "'", 24, 0);
        assertTokenAt(24, EToken::DQuote, "\"", 25, 0);
        assertTokenAt(25, EToken::QMark, "?", 26, 0);
        assertTokenAt(26, EToken::Tilde, "~", 27, 0);
        assertTokenAt(27, EToken::Dollar, "$", 28, 0);
        assertTokenAt(28, EToken::At, "@", 29, 0);
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
        assertTokenAt(0, EToken::ExclaimEqual, "!=", 1, 0);
        assertTokenAt(1, EToken::StarEqual, "*=", 2, 0);
        assertTokenAt(2, EToken::AmpersandEqual, "&=", 3, 0);
        assertTokenAt(3, EToken::BarEqual, "|=", 4, 0);
        assertTokenAt(4, EToken::HatEqual, "^=", 5, 0);
        assertTokenAt(5, EToken::FwdSlashEqual, "/=", 6, 0);
        assertTokenAt(6, EToken::PercentEqual, "%=", 7, 0);
        assertTokenAt(7, EToken::GreaterEqual, ">=", 8, 0);
        assertTokenAt(8, EToken::LessEqual, "<=", 9, 0);
        assertTokenAt(9, EToken::PlusEqual, "+=", 10, 0);
        assertTokenAt(10, EToken::MinusEqual, "-=", 11, 0);
        assertTokenAt(11, EToken::BarGreater, "|>", 12, 0);
        assertTokenAt(12, EToken::MinusGreater, "->", 13, 0);
        assertTokenAt(13, EToken::EqualGreater, "=>", 14, 0);
        assertTokenAt(14, EToken::Colon2, "::", 15, 0);
        assertTokenAt(15, EToken::Star2, "**", 16, 0);
        assertTokenAt(16, EToken::Ampersand2, "&&", 17, 0);
        assertTokenAt(17, EToken::Bar2, "||", 18, 0);
        assertTokenAt(18, EToken::Plus2, "++", 19, 0);
        assertTokenAt(19, EToken::Minus2, "--", 20, 0);
        assertTokenAt(20, EToken::Equal2, "==", 21, 0);
        assertTokenAt(21, EToken::Greater2, ">>", 22, 0);
        assertTokenAt(22, EToken::Less2, "<<", 23, 0);
        assertTokenAt(23, EToken::QMark2, "??", 24, 0);
        assertTokenAt(24, EToken::Dot2, "..", 25, 0);
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
        assertTokenAt(0, EToken::ColonEqualGreater, ":=>", 1, 0);
        assertTokenAt(1, EToken::ColonTildeGreater, ":~>", 2, 0);
        assertTokenAt(2, EToken::Greater2Equal, ">>=", 3, 0);
        assertTokenAt(3, EToken::Less2Equal, "<<=", 4, 0);
        assertTokenAt(4, EToken::Star2Equal, "**=", 5, 0);
        assertTokenAt(5, EToken::Dot3, "...", 6, 0);
    }
}
