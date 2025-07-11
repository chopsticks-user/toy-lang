#ifndef  TLC_CORE_CONCEPT_HPP
#define  TLC_CORE_CONCEPT_HPP

#include <concepts>

namespace tlc {
    template <typename TDerived, typename TBase>
    concept CIsChildOf = std::derived_from<TDerived, TBase> &&
        !std::same_as<TDerived, TBase>;

    template <typename T, typename... TExpected>
    concept CIsEither = (std::same_as<T, TExpected> || ...);
};

#endif // TLC_CORE_CONCEPT_HPP
