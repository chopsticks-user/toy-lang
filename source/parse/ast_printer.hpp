#ifndef  TLC_SYNTAX_AST_PRINTER_HPP
#define  TLC_SYNTAX_AST_PRINTER_HPP

#include "core/core.hpp"
#include "syntax/nodes.hpp"
#include "syntax/visitor.hpp"

namespace tlc::parse {
    class ASTPrinter final : public syntax::SyntaxTreeVisitor<Str> {
        static constexpr StrV prefixSymbol = "├─ ";
        static constexpr StrV spaceSymbol = "   ";
        static constexpr Str emptyNodeStr = "(empty)";

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

        auto operator()(syntax::type::Identifier const& node) -> Str;
        auto operator()(syntax::type::Array const& node) -> Str;
        auto operator()(syntax::type::Tuple const& node) -> Str;
        auto operator()(syntax::type::Function const& node) -> Str;
        auto operator()(syntax::type::Infer const& node) -> Str;

        auto operator()(syntax::decl::Identifier const& node) -> Str;
        auto operator()(syntax::decl::Tuple const& node) -> Str;

        auto operator()(syntax::stmt::Return const& node) -> Str;
        auto operator()(syntax::stmt::Yield const& node) -> Str;
        auto operator()(syntax::stmt::Let const& node) -> Str;
        auto operator()(syntax::stmt::Expression const& node) -> Str;
        auto operator()(syntax::stmt::Assign const& node) -> Str;
        auto operator()(syntax::stmt::Conditional const& node) -> Str;

    private:
        static constexpr auto rvFilterEmpty =
            rv::filter([](auto const& s) {
                return !s.empty();
            });

        static constexpr auto rvTransformEmpty =
            rv::transform([](auto const& s) {
                return s.empty() ? emptyNodeStr : s;
            });

        [[nodiscard]] auto withDepth(Str s) const -> Str;
    };
}

#endif // TLC_SYNTAX_AST_PRINTER_HPP
