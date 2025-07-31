// ReSharper disable CppMemberFunctionMayBeConst
#include "ast_printer.hpp"
#include "syntax/visitor.hpp"
#include "syntax/util.hpp"

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
                  ? "\n" + (visitChildren(node) | rvJoinWithEl)
                  : "");
    }

    auto ASTPrinter::operator()(syntax::expr::Tuple const& node) -> Str {
        return withDepth(std::format(
            "expr::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (node.size() != 0
                  ? "\n" + (visitChildren(node) | rvJoinWithEl)
                  : "");
    }

    auto ASTPrinter::operator()(syntax::expr::FnApp const& node) -> Str {
        return withDepth(std::format(
            "expr::FnApp [@{}:{}]", node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::expr::Subscript const& node) -> Str {
        return withDepth(std::format(
            "expr::Subscript [@{}:{}]", node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
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
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::expr::Record const& node) -> Str {
        return withDepth(std::format(
            "expr::Record [@{}:{}] with keys = [{}]",
            node.line(), node.column(), node.keys() | rv::transform(
                [](auto const& k) { return std::format("'{}'", k); }
            ) | rv::join_with(',') | rng::to<Str>()
        )) + "\n" + (
            visitChildren(node) | rvFilterEmpty
            | rvJoinWithEl
        );
    }

    auto ASTPrinter::operator()(syntax::type::Identifier const& node) -> Str {
        return withDepth(std::format(
            "type::Identifier [@{}:{}] with (fundamental, path) = ({:s}, '{}')",
            node.line(), node.column(), node.fundamental(), node.path()
        ));
    }

    auto ASTPrinter::operator()(syntax::type::Array const& node) -> Str {
        return withDepth(std::format(
            "type::Array [@{}:{}] with dims = {{{}}}",
            node.line(), node.column(),
            rv::iota(0ull, node.nDims()) | rv::transform([&node](auto index) {
                return std::to_string(index)
                    + (node.fixed(index) ? ":fix" : ":dyn");
            }) | rng::to<Vec<Str>>() | rv::join_with(',') | rng::to<Str>()
        )) + (node.nChildren() > 0
                  ? "\n" + (
                      visitChildren(node) | rvFilterEmpty | rvJoinWithEl
                  )
                  : "");
    }

    auto ASTPrinter::operator()(syntax::type::Tuple const& node) -> Str {
        return withDepth(std::format(
            "type::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (node.size() > 0
                  ? "\n" + (visitChildren(node) | rvJoinWithEl)
                  : "");
    }

    auto ASTPrinter::operator()(syntax::type::Function const& node) -> Str {
        return withDepth(std::format(
            "type::Function [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::type::Infer const& node) -> Str {
        return withDepth(std::format(
            "type::Infer [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::decl::Identifier const& node) -> Str {
        return withDepth(std::format(
            "decl::Identifier [@{}:{}] with (const, name) = ({}, '{}')",
            node.line(), node.column(), node.constant(), node.name()
        )) + (node.inferred() ? "" : "\n" + visitChildren(node).front());
    }

    auto ASTPrinter::operator()(syntax::decl::Tuple const& node) -> Str {
        return withDepth(std::format(
            "decl::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (node.size() > 0
                  ? "\n" + (visitChildren(node) | rvJoinWithEl)
                  : "");
    }

    auto ASTPrinter::operator()(syntax::stmt::Return const& node) -> Str {
        Str suffix = visitChildren(node).front();
        if (!suffix.empty()) {
            suffix = std::format("\n{}", suffix);
        }

        return withDepth(std::format(
            "stmt::Return [@{}:{}]",
            node.line(), node.column()
        )) + suffix;
    }

    auto ASTPrinter::operator()(syntax::stmt::Yield const& node) -> Str {
        Str suffix = visitChildren(node).front();
        if (!suffix.empty()) {
            suffix = std::format("\n{}", suffix);
        }

        return withDepth(std::format(
            "stmt::Yield [@{}:{}]",
            node.line(), node.column()
        )) + suffix;
    }

    auto ASTPrinter::operator()(syntax::stmt::Let const& node) -> Str {
        auto suffix = visitChildren(node) | rvFilterEmpty
            | rvJoinWithEl;
        if (!suffix.empty()) {
            suffix = std::format("\n{}", suffix);
        }

        return withDepth(std::format(
            "stmt::Let [@{}:{}]",
            node.line(), node.column()
        )) + suffix;
    }

    auto ASTPrinter::operator()(syntax::stmt::Expression const& node) -> Str {
        auto suffix = visitChildren(node).front();
        return withDepth(std::format(
            "stmt::Expression [@{}:{}]",
            node.line(), node.column()
        )) + Str(suffix.empty() ? emptyNodeStr : std::format("\n{}", suffix));
    }

    auto ASTPrinter::operator()(syntax::stmt::Assign const& node) -> Str {
        return withDepth(std::format(
            "stmt::Assign [@{}:{}] with op = '{}'",
            node.line(), node.column(), token::reversedOperatorTable.at(node.op())
        )) + ("\n" + (visitChildren(node) | rvTransformEmpty |
                rvJoinWithEl)
        );
    }

    auto ASTPrinter::operator()(syntax::stmt::Conditional const& node) -> Str {
        return withDepth(std::format(
            "stmt::Conditional [@{}:{}]",
            node.line(), node.column()
        )) + ("\n" + (visitChildren(node) | rvTransformEmpty |
                rvJoinWithEl)
        );
    }

    auto ASTPrinter::operator()(syntax::stmt::Block const& node) -> Str {
        return withDepth(std::format(
            "stmt::Block [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + (
            node.size() == 0
                ? ""
                : "\n" + (visitChildren(node) | rvJoinWithEl)
        );
    }

    auto ASTPrinter::operator()(syntax::stmt::Preface const& node) -> Str {
        return withDepth(std::format(
            "stmt::Preface [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::stmt::Defer const& node) -> Str {
        return withDepth(std::format(
            "stmt::Defer [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::stmt::Loop const& node) -> Str {
        return withDepth(std::format(
            "stmt::Loop [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::stmt::MatchCase const& node) -> Str {
        return withDepth(std::format(
            "stmt::MatchCase [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvFilterEmpty | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::stmt::Match const& node) -> Str {
        return withDepth(std::format(
            "stmt::Match [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::withDepth(Str s) const -> Str {
        return (
            [&] -> Vec<StrV> {
                switch (auto const d = depth(); d) {
                case 0: return {};
                case 1: return {prefixSymbol};
                default: {
                    // todo: fix rv::concat impl
                    auto v = Vec(d, space);
                    v.back() = prefixSymbol;
                    return v;
                }
                }
            }() | rv::join | rng::to<Str>()
        ) + std::move(s);
    }
}
