#ifndef  TLC_SYNTAX_PRETTY_PRINTER_HPP
#define  TLC_SYNTAX_PRETTY_PRINTER_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

namespace tlc::parse {
    class PrettyPrint final : public syntax::SyntaxTreeVisitor<Str> {
        static constexpr StrV indent = "    ";
        static constexpr StrV empty = "{?}";
        static constexpr StrV required = "{!}";

    public:
        using SyntaxTreeVisitor::operator();

        static auto operator()(syntax::Node const& node) -> Str {
            return std::visit(PrettyPrint{}, node);
        }

    public:
        auto operator()(syntax::expr::Integer const& node) -> Str;
        auto operator()(syntax::expr::Float const& node) -> Str;
        auto operator()(syntax::expr::Boolean const& node) -> Str;
        auto operator()(syntax::expr::Identifier const& node) -> Str;
        auto operator()(syntax::expr::Tuple const& node) -> Str;
        auto operator()(syntax::expr::Array const& node) -> Str;

        auto operator()(std::monostate const&) -> Str;
        auto operator()(syntax::RequiredButMissing const&) -> Str;

    private:
        static constexpr auto rvJoinWithComma =
            rv::join_with(", "sv) | rng::to<Str>();

        [[nodiscard]] auto withDepth(StrV s) const -> Str;
    };
}

#endif // TLC_SYNTAX_PRETTY_PRINTER_HPP
