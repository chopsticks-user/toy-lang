// ReSharper disable CppMemberFunctionMayBeConst
#include "pretty_printer.hpp"

namespace tlc::parse {
    auto PrettyPrint::operator()(syntax::expr::Integer const& node) -> Str {
        return withDepth(std::format("{}", node.value()));
    }

    auto PrettyPrint::operator()(syntax::expr::Float const& node) -> Str {
        return withDepth(std::format("{}", node.value()));
    }

    auto PrettyPrint::operator()(syntax::expr::Boolean const& node) -> Str {
        return withDepth(std::format("{:s}", node.value()));
    }

    auto PrettyPrint::operator()(syntax::expr::Identifier const& node) -> Str {
        return withDepth(std::format("{}", node.path()));
    }

    auto PrettyPrint::withDepth(StrV s) const -> Str {
        return (
            [&] -> Vec<StrV> {
                switch (auto const d = depth(); d) {
                case 0: return {};
                case 1: return {indent};
                default: return Vec(d, indent);
                }
            }() | rv::join | rng::to<Str>()
        ) + static_cast<Str>(std::move(s));
    }
}
