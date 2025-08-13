#ifndef  TLC_CORE_SINGLETON_HPP
#define  TLC_CORE_SINGLETON_HPP

#include "type.hpp"
#include "concept.hpp"

namespace tlc {
    template <typename T>
    concept IsSingleton =
        !IsExternallyConstructible<RemoveAllQualifiers<T>> &&
        !IsAssignable<RemoveAllQualifiers<T>> &&
        std::is_destructible_v<RemoveAllQualifiers<T>>;

    class Singleton {
    public:
        Singleton& operator=(const Singleton&) = delete;

        Singleton& operator=(Singleton&&) = delete;

        template <IsSingleton T>
        static auto instance() -> RemoveAllQualifiers<T>& {
            struct Instance : T {
                Instance() = default;
                ~Instance() = default;
            };
            static std::once_flag onceFlag;
            static Ptr<T> ptr;

            if (!ptr) {
                std::call_once(onceFlag, [&]() -> void {
                    ptr = std::make_unique<Instance>();
                });
            }
            return *ptr;
        }

    protected:
        Singleton() = default;
    };
}

#endif // TLC_CORE_SINGLETON_HPP
