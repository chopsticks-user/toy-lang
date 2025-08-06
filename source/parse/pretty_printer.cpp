// ReSharper disable CppMemberFunctionMayBeConst
#include "pretty_printer.hpp"

namespace tlc::parse {
    auto PrettyPrint::operator()(syntax::expr::Integer const& node) -> Str {
        return std::format("{}", node.value());
    }

    auto PrettyPrint::operator()(syntax::expr::Float const& node) -> Str {
        return std::format("{}", node.value());
    }

    auto PrettyPrint::operator()(syntax::expr::Boolean const& node) -> Str {
        return std::format("{:s}", node.value());
    }

    auto PrettyPrint::operator()(syntax::expr::Identifier const& node) -> Str {
        return node.path();
    }

    auto PrettyPrint::operator()(syntax::expr::Tuple const& node) -> Str {
        return std::format("({})", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::expr::Array const& node) -> Str {
        return std::format("[{}]", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::expr::RecordEntry const& node) -> Str {
        return std::format(
            "{}: {}", node.key(), visitChildren(node).front()
        );
    }

    auto PrettyPrint::operator()(syntax::expr::Record const& node) -> Str {
        auto [type, entries] = [&]-> Pair<Str, Str> {
            auto children = visitChildren(node);
            // specify {Str} to make sure {first} is not a reference
            Str first = children.front();
            return {
                std::move(first),
                std::move(children) | rv::drop(1) | rvJoinWithComma
            };
        }();
        return std::format("{}{{{}}}", type, entries);
    }

    auto PrettyPrint::operator()(syntax::expr::Try const& node) -> Str {
        return std::format("try {}", visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::expr::String const& node) -> Str {
        auto [fragmentsExceptLast, lastFragment] =
            [&] {
                auto const fragments = node.fragments();
                Str last = fragments.back();
                return std::make_pair(
                    fragments.first(fragments.size() - 1), std::move(last)
                );
            }();
        return std::format(
            "\"{}{}\"",
            rv::zip(fragmentsExceptLast, visitChildren(node))
            | rv::transform([](std::tuple<Str, Str> const& t) {
                auto const& [fragment, placeholder] = t;
                return std::format("{}{{{}}}", fragment, placeholder);
            })
            | rv::join | rng::to<Str>(), lastFragment
        );
    }

    auto PrettyPrint::operator()(syntax::expr::FnApp const& node) -> Str {
        auto children = visitChildren(node);
        return std::format(
            "{}{}", children.front(),
            children | rv::as_rvalue | rv::drop(1) | rvJoinWithComma
        );
    }

    auto PrettyPrint::operator()(syntax::expr::Subscript const& node) -> Str {
        auto children = visitChildren(node);
        return std::format(
            "{}{}", children.front(),
            children | rv::as_rvalue | rv::drop(1) | rvJoinWithComma
        );
    }

    auto PrettyPrint::operator()(syntax::expr::Prefix const& node) -> Str {
        return std::format(
            "{}{}", node.op().str(), visitChildren(node).front()
        );
    }

    auto PrettyPrint::operator()(syntax::expr::Binary const& node) -> Str {
        return std::format(
            "({})", visitChildren(node) | rv::join_with(
                std::format(" {} ", node.op().str())
            ) | rng::to<Str>()
        );
    }

    auto PrettyPrint::operator()(syntax::type::Identifier const& node) -> Str {
        return node.path();
    }

    auto PrettyPrint::operator()(syntax::Empty const&) -> Str {
        return Str{empty};
    }

    auto PrettyPrint::operator()(syntax::RequiredButMissing const&) -> Str {
        return Str{required};
    }

    // auto PrettyPrint::withDepth(StrV s) const -> Str {
    //     // return (
    //     //     [&] -> Vec<StrV> {
    //     //         switch (auto const d = depth(); d) {
    //     //         case 0: return {};
    //     //         case 1: return {indent};
    //     //         default: return Vec(d, indent);
    //     //         }
    //     //     }() | rv::join | rng::to<Str>()
    //     // ) + static_cast<Str>(s);
    //     return "";
    // }
}
