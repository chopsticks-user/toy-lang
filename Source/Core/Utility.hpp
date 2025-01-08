#ifndef  TOYLANG_CORE_UTILITY_HPP
#define  TOYLANG_CORE_UTILITY_HPP

#include "Types.hpp"

namespace tl::util {
  template<typename Callable, typename... Args>
  auto apply(Args &&... args) {
    // forward_as_tuple as opposed to make_tuple to avoid copying
    return std::apply(Callable{}, std::forward_as_tuple(std::forward<Args>(args)...));
  }
}

#endif // TOYLANG_CORE_UTILITY_HPP
