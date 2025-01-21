#ifndef  TOYLANG_FRONTEND_COMB_PARSER_HPP
#define  TOYLANG_FRONTEND_COMB_PARSER_HPP

#include "Syntax/Syntax.hpp"
#include "Lexer.hpp"

#include "Core/Core.hpp"

namespace tl::fe {
  class CParsingDriver final {
  public:
    enum class Context {
      Id, TypeId, AnonId
    };

    CParsingDriver() = default;

    CParsingDriver(String filepath, Tokens tokens)
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

    auto markRevertPoint() noexcept -> void {
      m_revertPoint = m_current;
    }

    auto toRevertPoint() noexcept -> void {
      if (!m_revertPoint.has_value()) {
        return;
      }

      m_current = m_revertPoint.value();
      m_revertPoint = {};
    }

    auto done() const noexcept -> bool {
      return m_current >= m_tokens.size();
    }

  private:
    auto currentBoundCheck() const -> void {
      if (done()) {
        throw InternalException(m_filepath, "Parser: m_current + 1 >= m_tokens.size()");
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
    Opt<sz> m_revertPoint;
    syntax::Storage m_currentStorage = syntax::Storage::Internal;
    GlobalExpceptionCollector *m_eCollector = nullptr; // todo: global singleton
  };

  class UnitParser {
  public:
    using TReturn = std::tuple<Vec<syntax::ASTNode>, Strings>;
    using TParseFn = std::function<TReturn(CParsingDriver &)>;
    using TThenFn = std::function<TReturn(CRef<Vec<syntax::ASTNode> >, CRef<Strings>)>;

    explicit UnitParser(TParseFn fn)
      : m_fn(std::move(fn)) {
    }

    auto operator()(CParsingDriver &driver) const -> TReturn {
      return m_fn(driver);
    }

    // auto then(TThenFn fn) const -> UnitParser;

  private:
    std::function<TReturn(CParsingDriver &)> m_fn;
  };

  class CParser final {
  public:
    auto operator()(String filepath, Tokens tokens)
      -> std::tuple<syntax::TranslationUnit, Strings>;

  private:
    CParsingDriver m_driver;
  };

  auto match(std::same_as<EToken> auto... expected) -> UnitParser;

  auto any(std::same_as<UnitParser> auto... parsers) -> UnitParser;

  auto sequence(std::same_as<UnitParser> auto... parsers) -> UnitParser;

  auto maybe(const UnitParser &parser) -> UnitParser;

  auto many0(const UnitParser &parser) -> UnitParser;

  auto many1(const UnitParser &parser) -> UnitParser;

  auto list(
    const UnitParser &parser, EToken separator = EToken::Comma, bool trailing = true
  ) -> UnitParser;

  // auto map(std::same_as<UnitParser> auto parser) -> UnitParser;

  auto integerExpr() -> UnitParser;

  auto floatExpr() -> UnitParser;

  auto booleanExpr() -> UnitParser;

  auto stringExpr() -> UnitParser;

  auto variableIdExpr() -> UnitParser;

  auto typeIdExpr() -> UnitParser;

  auto typeExpr() -> UnitParser;

  auto arrayExpr() -> UnitParser;

  auto tupleExpr() -> UnitParser;

  auto lambdaExpr() -> UnitParser;

  auto fnCallExpr() -> UnitParser;

  auto subscriptExpr() -> UnitParser;

  auto accessExpr() -> UnitParser;

  auto primaryExpr() -> UnitParser;

  auto postfixExpr() -> UnitParser;

  auto prefixExpr() -> UnitParser;

  auto exponentialExpr() -> UnitParser;

  auto multiplicativeExpr() -> UnitParser;

  auto additiveExpr() -> UnitParser;

  auto shiftExpr() -> UnitParser;

  auto relationalExpr() -> UnitParser;

  auto equalityExpr() -> UnitParser;

  auto andExpr() -> UnitParser;

  auto exclusiveOrExpr() -> UnitParser;

  auto inclusiveOrExpr() -> UnitParser;

  auto logicalAndExpr() -> UnitParser;

  auto logicalOrExpr() -> UnitParser;

  auto nullCoalescingExpr() -> UnitParser;

  auto sequenceExpr() -> UnitParser;

  auto ternaryExpr() -> UnitParser;

  auto expr() -> UnitParser;

  auto identifierDecl() -> UnitParser;

  auto tupleDecl() -> UnitParser;

  auto stmt() -> UnitParser;

  auto translationUnit() -> UnitParser;
}

#endif // TOYLANG_FRONTEND_COMB_PARSER_HPP
