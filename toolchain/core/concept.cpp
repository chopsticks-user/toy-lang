export module core:concept_;

import :fwd;

export namespace tlc {
    template <typename Derived, typename Base>
    concept IsChildOf = std::derived_from<Derived, Base> &&
        !std::same_as<Derived, Base>;

    template <typename T, typename... Expected>
    concept IsEither = (std::same_as<T, Expected> || ...);

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
    using RemoveAllQualifiers = std::remove_cvref_t<std::remove_pointer_t<T>>;
};
