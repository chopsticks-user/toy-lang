#ifndef TLC_CORE_CONCEPT_HPP
#define TLC_CORE_CONCEPT_HPP

#include "core.fwd.hpp"

namespace tlc {
    template <typename T>
    using CanonicalTypeOf = std::remove_cvref_t<T>;

    template <typename T>
    concept IsCanonical = std::is_same_v<CanonicalTypeOf<T>, T>;

    template <typename T>
    concept IsNonVoid = !std::same_as<CanonicalTypeOf<T>, void>;

    template <typename Derived, typename Base>
    concept IsChildOf =
        std::derived_from<CanonicalTypeOf<Derived>, Base> &&
        !std::same_as<CanonicalTypeOf<Derived>, Base>;

    template <typename T, typename... Expected>
    concept MatchesAnyOf =
        (std::same_as<T, Expected> or ...);

    template <typename T, typename... Expected>
    concept CanonicallyMatchesAnyOf =
        (std::same_as<CanonicalTypeOf<T>, Expected> or ...);

    template <typename T>
    concept IsMovable =
        std::is_move_constructible_v<T> &&
        std::is_move_assignable_v<T>;

    template <typename T>
    concept IsCopyable =
        std::is_copy_constructible_v<T> &&
        std::is_copy_assignable_v<T>;

    template <typename T>
    concept IsExternallyConstructible =
        std::is_default_constructible_v<T> or
        std::is_constructible_v<T> or
        std::is_move_constructible_v<T> or
        std::is_copy_constructible_v<T>;

    template <typename T>
    concept IsAssignable =
        std::is_move_assignable_v<T> or
        std::is_copy_assignable_v<T>;
}

#endif // TLC_CORE_CONCEPT_HPP
