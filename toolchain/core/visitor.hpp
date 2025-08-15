#ifndef  TLC_CORE_VISITOR_HPP
#define  TLC_CORE_VISITOR_HPP

#include "core.fwd.hpp"

namespace tlc {
    template <typename TVisitable, typename TReturn = void>
        requires std::is_default_constructible_v<TReturn> || std::same_as<TReturn, void>
    class Visitor {
    public:
        using Visitable = TVisitable;

        constexpr auto operator()(std::convertible_to<TVisitable> auto const&)
            -> auto {
            if constexpr (!std::is_void_v<TReturn>) {
                return TReturn{};
            }
        }

    protected:
        constexpr Visitor() = default;
    };

    template <typename T>
    concept IsVisitor =
        requires(T t) // inherits from Visitor<...>
        {
            []<typename TVisitable, typename TReturn = void>(
            Visitor<TVisitable, TReturn>) {}(t);
        } &&
        requires(T) // the typename field Visitable is accessible
        {
            typename T::Visitable;
        };

    template <IsVisitor TVisitor>
    auto visit(std::convertible_to<typename TVisitor::Visitable> auto&& visitable) {
        return std::visit(TVisitor{}, std::forward<typename TVisitor::Visitable>(visitable));
    }
}

#endif // TLC_CORE_VISITOR_HPP
