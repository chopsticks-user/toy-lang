#ifndef TLC_CORE_MIXIN_HPP
#define TLC_CORE_MIXIN_HPP

#include "type.hpp"
#include "concept.hpp"

namespace tlc {
    template <typename T>
    class MoveOnly {
    public:
        MoveOnly() noexcept { staticEnforce(); }
        ~MoveOnly() noexcept = default;
        MoveOnly(MoveOnly&&) noexcept = default;
        MoveOnly(MoveOnly const&) noexcept = delete;
        MoveOnly& operator=(MoveOnly&&) noexcept = default;
        MoveOnly& operator=(MoveOnly const&) noexcept = delete;

    private:
        static constexpr auto staticEnforce() noexcept -> void {
            static_assert(IsMovable<T>);
        }
    };

    template <typename T>
    class Default {
    public:
        Default() noexcept { staticEnforce(); }
        ~Default() noexcept = default;
        Default(Default&&) noexcept = default;
        Default(Default const&) noexcept = default;
        Default& operator=(Default&&) noexcept = default;
        Default& operator=(Default const&) noexcept = default;

    private:
        static constexpr auto staticEnforce() noexcept -> void {
            static_assert(std::is_default_constructible_v<T>);
        }
    };

    template <typename T>
    class NonDefault {
    public:
        NonDefault() noexcept { staticEnforce(); }
        ~NonDefault() noexcept = default;
        NonDefault(NonDefault&&) noexcept = default;
        NonDefault(NonDefault const&) noexcept = default;
        NonDefault& operator=(NonDefault&&) noexcept = default;
        NonDefault& operator=(NonDefault const&) noexcept = default;

    private:
        static constexpr auto staticEnforce() noexcept -> void {
            static_assert(!std::is_default_constructible_v<T>);
        }
    };

    // template <typename T>
    // class Copyable {
    // public:
    //     Copyable() noexcept = default;
    //     ~Copyable() noexcept = default;
    //     Copyable(Copyable&&) noexcept = default;
    //     Copyable(Copyable const&) noexcept = default;
    //     Copyable& operator=(Copyable&&) noexcept = default;
    //     Copyable& operator=(Copyable const&) noexcept = default;
    //
    // private:
    //     static constexpr auto staticEnforce() noexcept -> void {
    //         static_assert(IsCopyable<T>);
    //     }
    // };
}

#endif // TLC_CORE_MIXIN_HPP
