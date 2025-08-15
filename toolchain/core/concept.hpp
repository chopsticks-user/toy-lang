#ifndef TLC_CORE_CONCEPT_HPP
#define TLC_CORE_CONCEPT_HPP

#include "core.fwd.hpp"

namespace tlc {
    template <typename T>
    using RemoveAllQualifiers = std::remove_cvref_t<std::remove_pointer_t<T>>;

    template <typename Derived, typename Base>
    concept IsChildOf = std::derived_from<Derived, Base> &&
        !std::same_as<Derived, Base>;

    template <typename T, typename... Expected>
    concept IdentidyMatchesAnyOf =
        (std::same_as<RemoveAllQualifiers<T>, Expected> || ...);

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
        std::is_default_constructible_v<T> ||
        std::is_constructible_v<T> ||
        std::is_move_constructible_v<T> ||
        std::is_copy_constructible_v<T>;

    template <typename T>
    concept IsAssignable =
        std::is_move_assignable_v<T> ||
        std::is_copy_assignable_v<T>;

    template <typename T>
    concept IsIdentityType = std::is_same_v<RemoveAllQualifiers<T>, T>;

    template <typename T>
    concept IsNonVoid = !std::same_as<T, void>;
}

#endif // TLC_CORE_CONCEPT_HPP
