#include <iostream>
#include <string>
#include <vector>
#include <concepts>
#include <memory>
#include <ranges>

template<typename D, typename B>
concept ChildClassOf = std::derived_from<D, B> && !std::same_as<D, B>;

// template<typename F, typename P0, typename... Pn>
// concept HomogeneousVariadicFun =
//     (std::same_as<P0, Pn> || ...) &&
//     requires(F f, P0 p0, Pn... pn)
//     {
//       { f() } -> std::convertible_to<bool>;
//       { f(p0) } -> std::convertible_to<bool>;
//       { f(p0, pn...) } -> std::convertible_to<bool>;
//     }
//     ;

struct ParsingDriverBase {
};

template<typename T>
concept IParsingDriver =
    ChildClassOf<T, ParsingDriverBase> &&
    requires(T t)
{
  { t.advance() } -> std::same_as<void>;
  { t.revert() } -> std::same_as<void>;
  { t.match() } -> std::convertible_to<bool>;
  { t.match(0) } -> std::convertible_to<bool>;
  { t.match(0, 1) } -> std::convertible_to<bool>;
  { t.match(1, 0, -1) } -> std::convertible_to<bool>;
};

struct ExceptionCollectorBase {
};

template<typename T>
concept IExceptionCollector =
    ChildClassOf<T, ExceptionCollectorBase> &&
    requires(T t, std::runtime_error &e)
{
  { t.throwAll() } -> std::same_as<void>;
  { t.collect(e) } -> std::same_as<void>;
};

class ParsingDriver : public ParsingDriverBase {
public:
  auto advance() -> void {
  }

  auto revert() -> void {
  }

  auto match(std::same_as<int> auto... params) -> bool {
    return ((params == 10) || ...);
  }
};

class ExceptionCollector : public ExceptionCollectorBase {
public:
  auto throwAll() -> void {
  }

  auto collect(std::runtime_error &e) -> void {
  }
};

#define __tl_inject_param_impl(Contract, service) \
  Contract auto &&service

#define __tl_inject_param(Pair) \
  __tl_inject_param_impl Pair

#define __tl_inject_init_impl(Contract, service) \
  service

#define __tl_inject_init_member(Contract, service) \
  _##service

#define __tl_inject_init(Pair) \
  __tl_inject_init_member Pair( \
    std::make_unique<std::decay_t<decltype(__tl_inject_init_impl Pair)> >( \
      std::forward<std::decay_t<decltype(__tl_inject_init_impl Pair)> >(__tl_inject_init_impl Pair) \
    ) \
  )

// Helper macros to expand over variadic arguments
#define __tl_for_each(macro, ...) APPLY(macro, __VA_ARGS__)
#define __tl_apply(macro, ...) __VA_OPT__(__tl_expand(__tl_apply_impl(macro, __VA_ARGS__)))
#define __tl_apply_impl(macro, arg, ...) macro(arg) __VA_OPT__(, __tl_apply_impl(macro, __VA_ARGS__))
#define __tl_parens ()

#define __tl_expand(...) __tl_expand4(__tl_expand4(__tl_expand4(__tl_expand4(__VA_ARGS__))))
#define __tl_expand4(...) __tl_expand3(__tl_expand3(__tl_expand3(__tl_expand3(__VA_ARGS__))))
#define __tl_expand3(...) __tl_expand2(__tl_expand2(__tl_expand2(__tl_expand2(__VA_ARGS__))))
#define __tl_expand2(...) __tl_expand1(__tl_expand1(__tl_expand1(__tl_expand1(__VA_ARGS__))))
#define __tl_expand1(...) __VA_ARGS__

#define __tl_for_each(macro, ...)                                    \
  __VA_OPT__(__tl_expand(__tl_for_each_helper(macro, __VA_ARGS__)))
#define __tl_for_each_helper(macro, a1, ...)                         \
  macro(a1) __VA_OPT__(,__tl_for_each_again __tl_parens (macro, __VA_ARGS__))
#define __tl_for_each_again() __tl_for_each_helper

#define tlInject(Client, ...) \
  explicit Client(__tl_for_each(__tl_inject_param, __VA_ARGS__)) \
  : __tl_for_each(__tl_inject_init, __VA_ARGS__)

class Parser {
public:
  tlInject(Parser, (IParsingDriver, driver), (IExceptionCollector, eCollector)) {
  }

private:
  std::unique_ptr<ParsingDriverBase> _driver;
  std::unique_ptr<ExceptionCollectorBase> _eCollector;
};

int main() {
  auto parser = Parser{ParsingDriver{}, ExceptionCollector{}};
  return 0;
}
