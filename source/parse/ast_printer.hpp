#ifndef  TLC_SYNTAX_AST_PRINTER_HPP
#define  TLC_SYNTAX_AST_PRINTER_HPP

#include "core/core.hpp"
#include "syntax/nodes.hpp"
#include "syntax/visitor.hpp"

namespace tlc::parse {
    class ASTPrinter final : public syntax::SyntaxTreeVisitor<Str> {
        static constexpr Str prefixSymbol = "├─ ";
        static constexpr Str spaceSymbol = "   ";

    public:
        using SyntaxTreeVisitor::operator();
        using SyntaxTreeVisitor::Visitable;

        static auto operator()(syntax::Node node) -> Str {
            return tlc::visit<ASTPrinter>(std::move(node));
        }

    public:
        auto operator()(syntax::expr::Integer const& node) -> Str;
        auto operator()(syntax::expr::Float const& node) -> Str;
        auto operator()(syntax::expr::Boolean const& node) -> Str;
        auto operator()(syntax::expr::Identifier const& node) -> Str;
        auto operator()(syntax::expr::Array const& node) -> Str;
        auto operator()(syntax::expr::Tuple const& node) -> Str;
        auto operator()(syntax::expr::FnApp const& node) -> Str;
        auto operator()(syntax::expr::Subscript const& node) -> Str;
        auto operator()(syntax::expr::Access const& node) -> Str;
        auto operator()(syntax::expr::Prefix const& node) -> Str;
        auto operator()(syntax::expr::Binary const& node) -> Str;
        auto operator()(syntax::expr::Record const& node) -> Str;

    private:
        [[nodiscard]] auto withDepth(Str s) const -> Str;
    };
}

#endif // TLC_SYNTAX_AST_PRINTER_HPP
