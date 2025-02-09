#ifndef TLC_CORE_VISITOR_HPP
#define TLC_CORE_VISITOR_HPP

#include <functional>
#include <algorithm>

namespace tlc {
  template<typename Callable, typename... Args>
  constexpr auto apply(Args &&... args) {
    // forward_as_tuple as opposed to make_tuple to avoid copying
    return std::apply(Callable{}, std::forward_as_tuple(std::forward<Args>(args)...));
  }
}

#endif // TLC_CORE_VISITOR_HPP
