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

        static constexpr auto operator()(syntax::Node const& node) -> Str {
            return std::visit(PrettyPrint{}, node);
        }

    public:
        auto operator()(syntax::expr::Integer const& node) -> Str;
        auto operator()(syntax::expr::Float const& node) -> Str;
        auto operator()(syntax::expr::Boolean const& node) -> Str;
        auto operator()(syntax::expr::Identifier const& node) -> Str;
        auto operator()(syntax::expr::Tuple const& node) -> Str;
        auto operator()(syntax::expr::Array const& node) -> Str;
        auto operator()(syntax::expr::RecordEntry const& node) -> Str;
        auto operator()(syntax::expr::Record const& node) -> Str;
        auto operator()(syntax::expr::Try const& node) -> Str;
        auto operator()(syntax::expr::String const& node) -> Str;
        auto operator()(syntax::expr::FnApp const& node) -> Str;
        auto operator()(syntax::expr::Subscript const& node) -> Str;
        auto operator()(syntax::expr::Prefix const& node) -> Str;
        auto operator()(syntax::expr::Binary const& node) -> Str;

        auto operator()(syntax::type::Identifier const& node) -> Str;
        auto operator()(syntax::type::Infer const& node) -> Str;
        auto operator()(syntax::type::Array const& node) -> Str;
        auto operator()(syntax::type::Tuple const& node) -> Str;
        auto operator()(syntax::type::Function const& node) -> Str;
        auto operator()(syntax::type::GenericArguments const& node) -> Str;
        auto operator()(syntax::type::Generic const& node) -> Str;
        auto operator()(syntax::type::Binary const& node) -> Str;

        auto operator()(syntax::decl::Identifier const& node) -> Str;
        auto operator()(syntax::decl::Tuple const& node) -> Str;
        auto operator()(syntax::decl::GenericIdentifier const& node) -> Str;
        auto operator()(syntax::decl::GenericParameters const& node) -> Str;

        auto operator()(syntax::global::ModuleDecl const& node) -> Str;
        auto operator()(syntax::global::ImportDecl const& node) -> Str;
        auto operator()(syntax::global::ImportDeclGroup const& node) -> Str;

        auto operator()(syntax::Empty const&) -> Str;
        auto operator()(syntax::RequiredButMissing const&) -> Str;
        auto operator()(syntax::TranslationUnit const& node) -> Str;

    private:
        static constexpr auto rvJoinWithComma =
            rv::join_with(", "s) | rng::to<Str>();

        static constexpr auto rvJoin =
            rv::join | rng::to<Str>();

        // [[nodiscard]] auto withDepth(StrV s) const -> Str;
    };
}

#endif // TLC_SYNTAX_PRETTY_PRINTER_HPP
