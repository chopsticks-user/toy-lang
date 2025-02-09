#ifndef  TLC_CORE_SINGLETON_HPP
#define  TLC_CORE_SINGLETON_HPP

#include "type.hpp"

#include <mutex>

namespace tlc {
  template<typename T>
  class Singleton {
  public:
    Singleton &operator=(const Singleton &) = delete;

    Singleton &operator=(Singleton &&) = delete;

    static auto instance() -> T & requires (
      !std::is_default_constructible_v<T> &&
      !std::is_constructible_v<T> &&
      !std::is_move_constructible_v<T> &&
      !std::is_copy_constructible_v<T> &&
      !std::is_move_assignable_v<T> &&
      !std::is_copy_assignable_v<T> &&
      std::is_destructible_v<T>
    ) {
      static Ptr<T> ptr;
      if (!ptr) {
        std::call_once(initFlag, [&]() -> void {
          ptr = std::make_unique<TInstance>();
        });
      }
      return *ptr;
    }

  protected:
    Singleton() = default;

  private:
    struct TInstance : T {
      TInstance() = default;

      ~TInstance() = default;
    };

    inline static std::once_flag initFlag;
  };
}

#endif // TLC_CORE_SINGLETON_HPP
