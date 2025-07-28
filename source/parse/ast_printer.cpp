// ReSharper disable CppMemberFunctionMayBeConst
#include "ast_printer.hpp"
#include "syntax/visitor.hpp"

#include <format>

namespace tlc::parse {
    auto ASTPrinter::operator()(syntax::expr::Integer const& node) -> Str {
        return withDepth(std::format(
            "expr::Integer [@{}:{}] with value = {}",
            node.line(), node.column(), node.value()
        ));
    }

    auto ASTPrinter::operator()(syntax::expr::Float const& node) -> Str {
        return withDepth(std::format(
            "expr::Float [@{}:{}] with value = {}",
            node.line(), node.column(), node.value()
        ));
    }

    auto ASTPrinter::operator()(syntax::expr::Boolean const& node) -> Str {
        return withDepth(std::format(
            "expr::Boolean [@{}:{}] with value = {:s}",
            node.line(), node.column(), node.value()
        ));
    }

    auto ASTPrinter::operator()(syntax::expr::Identifier const& node) -> Str {
        return withDepth(std::format(
            "expr::Identifier [@{}:{}] with path = '{}'",
            node.line(), node.column(), node.path()
        ));
    }

    auto ASTPrinter::operator()(syntax::expr::Array const& node) -> Str {
        return withDepth(std::format(
            "expr::Array [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (node.size() != 0
                  ? "\n" + (visitChildren(node) | rv::join_with('\n') | rng::to<Str>())
                  : "");
    }

    auto ASTPrinter::operator()(syntax::expr::Tuple const& node) -> Str {
        return withDepth(std::format(
            "expr::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (node.size() != 0
                  ? "\n" + (visitChildren(node) | rv::join_with('\n') | rng::to<Str>())
                  : "");
    }

    auto ASTPrinter::operator()(syntax::expr::FnApp const& node) -> Str {
        return withDepth(std::format(
            "expr::FnApp [@{}:{}]", node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rv::join_with('\n') | rng::to<Str>());
    }

    auto ASTPrinter::operator()(syntax::expr::Subscript const& node) -> Str {
        return withDepth(std::format(
            "expr::Subscript [@{}:{}]", node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rv::join_with('\n') | rng::to<Str>());
    }

    auto ASTPrinter::operator()(syntax::expr::Access const& node) -> Str {
        return withDepth(std::format(
            "expr::Access [@{}:{}] with field = '{}'",
            node.line(), node.column(), node.field()
        )) + "\n" + visitChildren(node).front();
    }

    auto ASTPrinter::operator()(syntax::expr::Prefix const& node) -> Str {
        return withDepth(std::format(
            "expr::Prefix [@{}:{}] with op = '{}'",
            node.line(), node.column(), token::reversedOperatorTable.at(node.op())
        )) + "\n" + visitChildren(node).front();
    }

    auto ASTPrinter::operator()(syntax::expr::Binary const& node) -> Str {
        return withDepth(std::format(
            "expr::Binary [@{}:{}] with op = '{}'",
            node.line(), node.column(), token::reversedOperatorTable.at(node.op())
        )) + "\n" + (visitChildren(node) | rv::join_with('\n') | rng::to<Str>());
    }

    auto ASTPrinter::operator()(syntax::expr::Record const& node) -> Str {
        return withDepth(std::format(
            "expr::Record [@{}:{}] with keys = [{}]",
            node.line(), node.column(), node.keys() | rv::transform(
                [](auto const& k) { return std::format("'{}'", k); }
            ) | rv::join_with(',') | rng::to<Str>()
        )) + (node.size() > 0
                  ? "\n" + (visitChildren(node) | rv::filter([](auto const& value) {
                      return !value.empty();
                  }) | rv::join_with('\n') | rng::to<Str>())
                  : "");
    }

    auto ASTPrinter::withDepth(Str s) const -> Str {
        return (
            [&] -> Vec<Str> {
                switch (auto const d = depth(); d) {
                case 0: return {};
                case 1: return {prefixSymbol};
                default: {
                    // todo: fix rv::concat impl
                    auto v = Vec(d, spaceSymbol);
                    v.back() = prefixSymbol;
                    return v;
                }
                }
            }() | rv::join | rng::to<Str>()
        ) + std::move(s);
    }
}
