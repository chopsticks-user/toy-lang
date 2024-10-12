#ifndef  TOYLANG_CORE_UTILITY_HPP
#define  TOYLANG_CORE_UTILITY_HPP

#include "Types.hpp"

namespace tl::util {
  template<typename Callable, typename... Args>
  auto apply(Args &&... args) {
    return std::apply(Callable{}, std::make_tuple(std::forward<Args>(args)...));
  }
}

#endif // TOYLANG_CORE_UTILITY_HPP
