#ifndef  TLC_SYNTAX_AST_PRINTER_HPP
#define  TLC_SYNTAX_AST_PRINTER_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

namespace tlc::parse {
    class ASTPrinter final : public syntax::SyntaxTreeVisitor<Str> {
        static constexpr Str prefix = "├─ ";
        static constexpr StrV space = "   ";
        static constexpr StrV empty = "(empty)";
        static constexpr StrV required = "(required)";

    public:
        using SyntaxTreeVisitor::operator();

        static auto operator()(syntax::Node const& node) -> Str {
            return std::visit(ASTPrinter{}, node);
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
        auto operator()(syntax::expr::Prefix const& node) -> Str;
        auto operator()(syntax::expr::Binary const& node) -> Str;
        auto operator()(syntax::expr::RecordEntry const& node) -> Str;
        auto operator()(syntax::expr::Record const& node) -> Str;
        auto operator()(syntax::expr::String const& node) -> Str;
        auto operator()(syntax::expr::Try const& node) -> Str;

        auto operator()(syntax::type::Identifier const& node) -> Str;
        auto operator()(syntax::type::Array const& node) -> Str;
        auto operator()(syntax::type::Tuple const& node) -> Str;
        auto operator()(syntax::type::Function const& node) -> Str;
        auto operator()(syntax::type::Infer const& node) -> Str;
        auto operator()(syntax::type::GenericArguments const& node) -> Str;
        auto operator()(syntax::type::Generic const& node) -> Str;
        auto operator()(syntax::type::Binary const& node) -> Str;

        auto operator()(syntax::decl::Identifier const& node) -> Str;
        auto operator()(syntax::decl::Tuple const& node) -> Str;
        auto operator()(syntax::decl::GenericIdentifier const& node) -> Str;
        auto operator()(syntax::decl::GenericParameters const& node) -> Str;

        auto operator()(syntax::stmt::Return const& node) -> Str;
        auto operator()(syntax::stmt::Decl const& node) -> Str;
        auto operator()(syntax::stmt::Expression const& node) -> Str;
        auto operator()(syntax::stmt::Assign const& node) -> Str;
        auto operator()(syntax::stmt::Conditional const& node) -> Str;
        auto operator()(syntax::stmt::Block const& node) -> Str;
        auto operator()(syntax::stmt::Defer const& node) -> Str;
        auto operator()(syntax::stmt::Loop const& node) -> Str;
        auto operator()(syntax::stmt::MatchCase const& node) -> Str;
        auto operator()(syntax::stmt::Match const& node) -> Str;

        auto operator()(syntax::global::ModuleDecl const& node) -> Str;
        auto operator()(syntax::global::ImportDecl const& node) -> Str;
        auto operator()(syntax::global::ImportDeclGroup const& node) -> Str;
        auto operator()(syntax::global::FunctionPrototype const& node) -> Str;
        auto operator()(syntax::global::Function const& node) -> Str;

        auto operator()(syntax::Empty const&) -> Str;
        auto operator()(syntax::RequiredButMissing const&) -> Str;
        auto operator()(syntax::TranslationUnit const& node) -> Str;

    private:
        static constexpr auto rvJoinWithEl =
            rv::join_with('\n') | rng::to<Str>();

        auto visitChildren(auto node) -> Str;

    private:
        szt m_depth = 0;
    };
}

#endif // TLC_SYNTAX_AST_PRINTER_HPP
