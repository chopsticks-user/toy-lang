#ifndef  TOYLANG_FRONTEND_COMB_PARSER_HPP
#define  TOYLANG_FRONTEND_COMB_PARSER_HPP

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
    String m_filepath;
    Tokens m_tokens;
    sz m_current = 0;
    syntax::Storage m_currentStorage = syntax::Storage::Internal;
    GlobalExpceptionCollector *m_eCollector = nullptr; // todo: global singleton
  };

  // class UnitParserExpr {
  // public:
  //   static constexpr bool isLeaf = false;
  //
  // protected:
  //   template<typename U>
  //   using TOperand = std::conditional_t<U::isLeaf, CRef<U>, const U>;
  //
  //   using Super = UnitParserExpr;
  // };

  class UnitParser final {
  public:
    // static constexpr bool isLeaf = true;

    using TReturn = std::tuple<Vec<syntax::ASTNode>, Strings>;
    using TParseFn = std::function<TReturn(ParsingDriver &)>;
    using TThenFn = std::function<TReturn(CRef<Vec<syntax::ASTNode> >, CRef<Strings>)>;

    explicit UnitParser(TParseFn fn)
      : m_fn(std::move(fn)) {
    }

    auto operator()(ParsingDriver &driver) const -> TReturn {
      return m_fn(driver);
    }

    // auto then(TThenFn fn) const -> UnitParser;

  private:
    std::function<TReturn(ParsingDriver &)> m_fn;
  };

  class Parser final {
  public:
    auto operator()(String filepath, Tokens tokens)
      -> std::tuple<syntax::TranslationUnit, Strings>;

    static auto match(std::same_as<EToken> auto... expected) -> UnitParser;

    static auto any(std::same_as<UnitParser> auto... parsers) -> UnitParser;

    static auto sequence(std::same_as<UnitParser> auto... parsers) -> UnitParser;

    static auto maybe(const UnitParser &parser) -> UnitParser;

    static auto many0(const UnitParser &parser) -> UnitParser;

    static auto many1(const UnitParser &parser) -> UnitParser;

    static auto list(
      const UnitParser &parser, EToken separator = EToken::Comma, bool trailing = true
    ) -> UnitParser;

    // auto map(std::same_as<UnitParser> auto parser) -> UnitParser;

    static auto integerExpr() -> UnitParser;

    static auto floatExpr() -> UnitParser;

    static auto booleanExpr() -> UnitParser;

    static auto stringExpr() -> UnitParser;

    static auto varIdExpr() -> UnitParser;

    static auto typeIdExpr() -> UnitParser;

    static auto opIdExpr() -> UnitParser;

    static auto arrayExpr() -> UnitParser;

    static auto tupleExpr() -> UnitParser;

    static auto lambdaExpr() -> UnitParser;

    static auto fnCallExpr() -> UnitParser;

    static auto subscriptExpr() -> UnitParser;

    static auto accessExpr() -> UnitParser;

    static auto primaryExpr() -> UnitParser;

    static auto postfixExpr() -> UnitParser;

    static auto prefixExpr() -> UnitParser;

    static auto exponentialExpr() -> UnitParser;

    static auto multiplicativeExpr() -> UnitParser;

    static auto additiveExpr() -> UnitParser;

    static auto shiftExpr() -> UnitParser;

    static auto relationalExpr() -> UnitParser;

    static auto equalityExpr() -> UnitParser;

    static auto andExpr() -> UnitParser;

    static auto exclusiveOrExpr() -> UnitParser;

    static auto inclusiveOrExpr() -> UnitParser;

    static auto logicalAndExpr() -> UnitParser;

    static auto logicalOrExpr() -> UnitParser;

    static auto nullCoalescingExpr() -> UnitParser;

    static auto sequenceExpr() -> UnitParser;

    static auto ternaryExpr() -> UnitParser;

    static auto expr() -> UnitParser;

    static auto identifierDecl() -> UnitParser;

    static auto tupleDecl() -> UnitParser;

    static auto stmt() -> UnitParser;

    static auto translationUnit() -> UnitParser;

  private:
    ParsingDriver m_driver;
  };
}

#endif // TOYLANG_FRONTEND_COMB_PARSER_HPP
