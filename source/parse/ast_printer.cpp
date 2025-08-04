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
            node.line(), node.column(), node.op().str()
        )) + "\n" + visitChildren(node).front();
    }

    auto ASTPrinter::operator()(syntax::expr::Binary const& node) -> Str {
        return withDepth(std::format(
            "expr::Binary [@{}:{}] with op = '{}'",
            node.line(), node.column(), node.op().str()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::expr::RecordEntry const& node) -> Str {
        return withDepth(std::format(
            "expr::RecordEntry [@{}:{}] with key = '{}'",
            node.line(), node.column(), node.key()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::expr::Record const& node) -> Str {
        return withDepth(std::format(
            "expr::Record [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + "\n" + (
            visitChildren(node) | rvFilterEmpty | rvJoinWithEl
        );
    }

    auto ASTPrinter::operator()(syntax::expr::String const& node) -> Str {
        return withDepth(std::format(
            "expr::String [@{}:{}] with nPlaceholders = {}",
            node.line(), node.column(), node.nPlaceholders()
        )) + (node.interpolated()
                  ? "\n" + (visitChildren(node) | rvFilterEmpty | rvJoinWithEl)
                  : "");
    }

    auto ASTPrinter::operator()(syntax::type::Identifier const& node) -> Str {
        return withDepth(std::format(
            "type::Identifier [@{}:{}] with (const, fund, path) "
            "= ({:s}, {:s}, '{}')",
            node.line(), node.column(), node.constant(),
            node.fundamental(), node.path()
        ));
    }

    auto ASTPrinter::operator()(syntax::type::Array const& node) -> Str {
        return withDepth(std::format(
            "type::Array [@{}:{}] with nDims = {}",
            node.line(), node.column(), node.nDims()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
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

    auto ASTPrinter::operator()(syntax::type::GenericArguments const& node) -> Str {
        return withDepth(std::format(
            "type::GenericArguments [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::type::Generic const& node) -> Str {
        return withDepth(std::format(
            "type::Generic [@{}:{}]",
            node.line(), node.column()
        )) + "\n" + (visitChildren(node) | rvJoinWithEl);
    }

    auto ASTPrinter::operator()(syntax::decl::Identifier const& node) -> Str {
        return withDepth(std::format(
            "decl::Identifier [@{}:{}] with name = '{}'",
            node.line(), node.column(), node.name()
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

    auto ASTPrinter::operator()(syntax::stmt::Decl const& node) -> Str {
        auto suffix = visitChildren(node) | rvFilterEmpty
            | rvJoinWithEl;
        if (!suffix.empty()) {
            suffix = std::format("\n{}", suffix);
        }

        return withDepth(std::format(
            "stmt::Decl [@{}:{}]",
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
            node.line(), node.column(), node.op().str()
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
