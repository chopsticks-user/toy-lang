#ifndef  TOYLANG_CORE_CONCEPTS_HPP
#define  TOYLANG_CORE_CONCEPTS_HPP

#include <concepts>

namespace tl {
  template<typename TDerived, typename TBase>
  concept IsChildOf = std::derived_from<TDerived, TBase> &&
                      !std::same_as<TDerived, TBase>;
};

#endif // TOYLANG_CORE_CONCEPTS_HPP
