#ifndef TOYLANG_LEXER_HPP
#define TOYLANG_LEXER_HPP

#include <utility>

#include "Token.hpp"

#include "Core/Core.hpp"

namespace tl::lexer {
  using Tokens = std::vector<Token>;

  /**
   * @brief
   */
  class Lexer final {
  public:
    auto operator()(std::string filepath) -> Tokens;

  private:
    static auto scanNumber(FileReader &reader) -> std::string;

    static auto scanIdentifier(FileReader &reader) -> std::string;

    static auto scanInteger(FileReader &reader) -> std::string;

    bool failed = false;
    sz m_currentLine = 0;
    sz m_currentColumn = 0;
  };
}

#endif // TOYLANG_LEXER_HPP
