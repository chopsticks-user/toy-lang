#ifndef  TOYLANG_FRONTEND_COMBINATORY_PARSER_HPP
#define  TOYLANG_FRONTEND_COMBINATORY_PARSER_HPP

#include "syntax/Syntax.hpp"
#include "Lexer.hpp"
#include "ParsingDriver.hpp"

#include "core/Core.hpp"

namespace tl::fe {
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

  class CombUnitParser final {
  public:
    // static constexpr bool isLeaf = true;

    using TReturn = std::tuple<Vec<syntax::ASTNode>, Strings>;
    using TParseFn = std::function<TReturn(ParsingDriver &)>;
    using TThenFn = std::function<TReturn(CRef<Vec<syntax::ASTNode> >, CRef<Strings>)>;

    explicit CombUnitParser(TParseFn fn)
      : m_fn(std::move(fn)) {
    }

    auto operator()(ParsingDriver &driver) const -> TReturn {
      return m_fn(driver);
    }

    // auto then(TThenFn fn) const -> UnitParser;

  private:
    std::function<TReturn(ParsingDriver &)> m_fn;
  };

  class CombParser final {
  public:
    auto operator()(String filepath, Tokens tokens)
      -> std::tuple<syntax::TranslationUnit, Strings>;

    static auto match(std::same_as<EToken> auto... expected) -> CombUnitParser;

    static auto any(std::same_as<CombUnitParser> auto... parsers) -> CombUnitParser;

    static auto sequence(std::same_as<CombUnitParser> auto... parsers) -> CombUnitParser;

    static auto maybe(const CombUnitParser &parser) -> CombUnitParser;

    static auto many0(const CombUnitParser &parser) -> CombUnitParser;

    static auto many1(const CombUnitParser &parser) -> CombUnitParser;

    static auto list(
      const CombUnitParser &parser, EToken separator = EToken::Comma, bool trailing = true
    ) -> CombUnitParser;

    // auto map(std::same_as<UnitParser> auto parser) -> UnitParser;

    static auto integerExpr() -> CombUnitParser;

    static auto floatExpr() -> CombUnitParser;

    static auto booleanExpr() -> CombUnitParser;

    static auto stringExpr() -> CombUnitParser;

    static auto varIdExpr() -> CombUnitParser;

    static auto typeIdExpr() -> CombUnitParser;

    static auto opIdExpr() -> CombUnitParser;

    static auto arrayExpr() -> CombUnitParser;

    static auto tupleExpr() -> CombUnitParser;

    static auto lambdaExpr() -> CombUnitParser;

    static auto fnCallExpr() -> CombUnitParser;

    static auto subscriptExpr() -> CombUnitParser;

    static auto accessExpr() -> CombUnitParser;

    static auto primaryExpr() -> CombUnitParser;

    static auto postfixExpr() -> CombUnitParser;

    static auto prefixExpr() -> CombUnitParser;

    static auto exponentialExpr() -> CombUnitParser;

    static auto multiplicativeExpr() -> CombUnitParser;

    static auto additiveExpr() -> CombUnitParser;

    static auto shiftExpr() -> CombUnitParser;

    static auto relationalExpr() -> CombUnitParser;

    static auto equalityExpr() -> CombUnitParser;

    static auto andExpr() -> CombUnitParser;

    static auto exclusiveOrExpr() -> CombUnitParser;

    static auto inclusiveOrExpr() -> CombUnitParser;

    static auto logicalAndExpr() -> CombUnitParser;

    static auto logicalOrExpr() -> CombUnitParser;

    static auto nullCoalescingExpr() -> CombUnitParser;

    static auto sequenceExpr() -> CombUnitParser;

    static auto ternaryExpr() -> CombUnitParser;

    static auto expr() -> CombUnitParser;

    static auto identifierDecl() -> CombUnitParser;

    static auto tupleDecl() -> CombUnitParser;

    static auto stmt() -> CombUnitParser;

    static auto translationUnit() -> CombUnitParser;

  private:
    ParsingDriver m_driver;
  };
}

#endif // TOYLANG_FRONTEND_COMBINATORY_PARSER_HPP
