#ifndef  TLC_CORE_CONCEPT_HPP
#define  TLC_CORE_CONCEPT_HPP

#include <concepts>

namespace tlc {
  template<typename TDerived, typename TBase>
  concept IsChildOf = std::derived_from<TDerived, TBase> &&
                      !std::same_as<TDerived, TBase>;
};

#endif // TLC_CORE_CONCEPT_HPP
