#include <catch2/catch_test_macros.hpp>

#include "lex/text_stream.cpp"

class TextStreamTestFixture {
protected:
    auto readFromSource(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_stream = tlc::lex::TextStream{std::move(iss)};
    }

    auto assertCurrentThenAdvance(
        tlc::c8 const c, tlc::szt const line, tlc::szt const column
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
    tlc::lex::TextStream m_stream;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(TextStreamTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("TextStream: Peek and match", "[Lex][TextStream]") {
    readFromSource(R"(lex
str eam)");

    // "lex"
    assertCurrentThenAdvance('l', 0, 0);
    assertCurrentThenAdvance('e', 0, 1);
    assertCurrentThenAdvance('x', 0, 2);
    assertCurrentThenAdvance('\n', 0, 3);

    // "stream"
    assertCurrentThenAdvance('s', 1, 0);
    assertCurrentThenAdvance('t', 1, 1);
    assertCurrentThenAdvance('r', 1, 2);
    assertCurrentThenAdvance(' ', 1, 3);
    assertCurrentThenAdvance('e', 1, 4);
    assertCurrentThenAdvance('a', 1, 5);
    assertCurrentThenAdvance('m', 1, 6);

    REQUIRE(done());
}
