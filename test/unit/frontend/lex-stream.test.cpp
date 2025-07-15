#include <catch2/catch_test_macros.hpp>

#include "lex/lex.hpp"

using tlc::lex::Stream;
using tlc::Str;
using tlc::szt;

class LexStreamTestFixture {
protected:
    auto readFromSource(Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_stream = Stream{std::move(iss)};
    }

    auto assertCurrent(
        char const c, szt const line, szt const column
    ) -> void {
        CAPTURE(c, line, column);
        REQUIRE(m_stream.peek() == c);
        REQUIRE(m_stream.match(c));
        REQUIRE(m_stream.current() == c);
        REQUIRE(m_stream.line() == line);
        REQUIRE(m_stream.column() == column);
    }

    [[nodiscard]] auto done() const -> bool {
        return m_stream.done();
    }

private:
    Stream m_stream;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(LexStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("LexStream: Peek and match", "[Lex]") {
    readFromSource(R"(lex
str eam)");

    // "lex"
    assertCurrent('l', 0, 0);
    assertCurrent('e', 0, 1);
    assertCurrent('x', 0, 2);
    assertCurrent('\n', 0, 3);

    // "stream"
    assertCurrent('s', 1, 0);
    assertCurrent('t', 1, 1);
    assertCurrent('r', 1, 2);
    assertCurrent(' ', 1, 3);
    assertCurrent('e', 1, 4);
    assertCurrent('a', 1, 5);
    assertCurrent('m', 1, 6);

    REQUIRE(done());
}
