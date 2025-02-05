#ifndef TOYLANG_FRONTEND_PARSING_DRIVER_HPP
#define TOYLANG_FRONTEND_PARSING_DRIVER_HPP

#include "Syntax/Syntax.hpp"
#include "Lexer.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class ParsingDriver final {
  public:
    enum class Context {
      Id, TypeId, AnonId
    };

    ParsingDriver() = default;

    ParsingDriver(String filepath, Tokens tokens)
      : m_filepath(std::move(filepath)), m_tokens(std::move(tokens)) {
    }

    auto filepath() -> CRef<String> {
      return m_filepath;
    }

    auto revert() -> void {
      backwardBoundCheck();
      --m_current;
    }

    auto advance() -> void {
      forwardBoundCheck();
      ++m_current;
    }

    auto to(sz index) -> void {
      // currentBoundCheck();
      m_current = index;
    }

    auto currentIndex() const -> sz {
      return m_current;
    }

    auto current() const -> CRef<Token> {
      currentBoundCheck();
      return m_tokens[m_current];
    }

    auto match(std::same_as<EToken> auto... expected) -> bool {
      if (done() || !((current().type() == expected) || ...)) {
        return false;
      }

      if (m_current < m_tokens.size()) {
        ++m_current;
      }
      return true;
    }

    auto peekNext() -> CRef<Token> {
      forwardBoundCheck();
      return m_tokens[m_current + 1];
    }

    auto peekPrev() -> CRef<Token> {
      backwardBoundCheck();
      return m_tokens[m_current - 1];
    }

    auto done() const noexcept -> bool {
      return m_current >= m_tokens.size();
    }

  private:
    auto currentBoundCheck() const -> void {
      if (done()) {
        throw InternalException(m_filepath, "Parser: m_current >= m_tokens.size()");
      }
    }

    auto forwardBoundCheck() const -> void {
      if (m_current + 1 >= m_tokens.size()) {
        throw InternalException(m_filepath, "Parser: m_current + 1 >= m_tokens.size()");
      }
    }

    auto backwardBoundCheck() const -> void {
      if (m_current - 1 < 0) {
        throw InternalException(m_filepath, "Parser: m_current - 1 < 0");
      }
    }

  private:
    String m_filepath = "";
    Tokens m_tokens = {};
    sz m_current = 0;
    syntax::Storage m_currentStorage = syntax::Storage::Internal;
  };
}

#endif // TOYLANG_FRONTEND_PARSING_DRIVER_HPP
