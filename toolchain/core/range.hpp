#ifndef TLC_CORE_RANGE_HPP
#define TLC_CORE_RANGE_HPP

#include "type.hpp"

namespace tlc {
    namespace rng = std::ranges;

    namespace rv {
        using namespace std::ranges::views;

        namespace detail {
            template <typename R>
            class ConcatView : public std::ranges::view_interface<ConcatView<R>> {
            public:
                ConcatView(R r1, R r2) : _r1(std::move(r1)), _r2(std::move(r2)) {}

                auto begin() {
                    return std::ranges::begin(_r1);
                }

                auto end() {
                    return std::ranges::end(_r2);
                }

            private:
                R _r1;
                R _r2;
            };

            template <typename R1, typename R2>
            class ViewConcatAdapter {
            public:
                ViewConcatAdapter(R1&& r1, R2&& r2)
                    : _r1(std::forward<R1>(r1)), _r2(std::forward<R2>(r2)) {}

                auto begin() { return std::ranges::begin(_r1); }
                auto end() { return std::ranges::end(_r2); }

            private:
                R1 _r1;
                R2 _r2;
            };
        }

        template <typename R1, typename R2>
        auto concat(R1&& r1, R2&& r2) {
            return detail::ConcatView(
                std::forward<R1>(r1), std::forward<R2>(r2)
            );
        }
    }

    template <rng::view R1, rng::view R2>
    auto operator|(R1&& r1, R2&& r2) {
        return rv::detail::ViewConcatAdapter<std::decay_t<R1>, std::decay_t<R2>>(
            std::forward<R1>(r1), std::forward<R2>(r2)
        );
    }
}

#endif // TLC_CORE_RANGE_HPP
