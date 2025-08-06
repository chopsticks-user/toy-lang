// ReSharper disable CppMemberFunctionMayBeConst
#include "ast_printer.hpp"

#include <format>

namespace tlc::parse {
    auto ASTPrinter::operator()(syntax::expr::Integer const& node) -> Str {
        return std::format(
            "expr::Integer [@{}:{}] with value = {}",
            node.line(), node.column(), node.value()
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Float const& node) -> Str {
        return std::format(
            "expr::Float [@{}:{}] with value = {}",
            node.line(), node.column(), node.value()
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Boolean const& node) -> Str {
        return std::format(
            "expr::Boolean [@{}:{}] with value = {:s}",
            node.line(), node.column(), node.value()
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Identifier const& node) -> Str {
        return std::format(
            "expr::Identifier [@{}:{}] with path = '{}'",
            node.line(), node.column(), node.path()
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Array const& node) -> Str {
        return std::format(
            "expr::Array [@{}:{}] with size = {}{}",
            node.line(), node.column(), node.size(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Tuple const& node) -> Str {
        return std::format(
            "expr::Tuple [@{}:{}] with size = {}{}",
            node.line(), node.column(), node.size(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::FnApp const& node) -> Str {
        return std::format(
            "expr::FnApp [@{}:{}]{}", node.line(), node.column(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Subscript const& node) -> Str {
        return std::format(
            "expr::Subscript [@{}:{}]{}", node.line(), node.column(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Prefix const& node) -> Str {
        return std::format(
            "expr::Prefix [@{}:{}] with op = '{}'{}",
            node.line(), node.column(), node.op().str(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::Binary const& node) -> Str {
        return std::format(
            "expr::Binary [@{}:{}] with op = '{}'",
            node.line(), node.column(), node.op().str()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::expr::RecordEntry const& node) -> Str {
        return std::format(
            "expr::RecordEntry [@{}:{}] with key = '{}'",
            node.line(), node.column(), node.key()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::expr::Record const& node) -> Str {
        return std::format(
            "expr::Record [@{}:{}] with size = {}{}",
            node.line(), node.column(), node.size(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::expr::String const& node) -> Str {
        return std::format(
            "expr::String [@{}:{}] with nPlaceholders = {}",
            node.line(), node.column(), node.nPlaceholders()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::expr::Try const& node) -> Str {
        return std::format(
            "expr::Try [@{}:{}]{}", node.line(), node.column(),
            this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::type::Identifier const& node) -> Str {
        return std::format(
            "type::Identifier [@{}:{}] with (const, fund, path) "
            "= ({:s}, {:s}, '{}')",
            node.line(), node.column(), node.constant(),
            node.fundamental(), node.path()
        );
    }

    auto ASTPrinter::operator()(syntax::type::Array const& node) -> Str {
        return std::format(
            "type::Array [@{}:{}] with nDims = {}",
            node.line(), node.column(), node.nDims()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::type::Tuple const& node) -> Str {
        return std::format(
            "type::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        ) + (node.size() > 0
                 ? this->visitChildren(node)
                 : "");
    }

    auto ASTPrinter::operator()(syntax::type::Function const& node) -> Str {
        return std::format(
            "type::Function [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::type::Infer const& node) -> Str {
        return std::format(
            "type::Infer [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::type::GenericArguments const& node) -> Str {
        return std::format(
            "type::GenericArguments [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::type::Generic const& node) -> Str {
        return std::format(
            "type::Generic [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::decl::Identifier const& node) -> Str {
        return std::format(
            "decl::Identifier [@{}:{}] with name = '{}'",
            node.line(), node.column(), node.name()
        ) + (node.inferred() ? "" : "\n" + this->visitChildren(node).front());
    }

    auto ASTPrinter::operator()(syntax::decl::Tuple const& node) -> Str {
        return std::format(
            "decl::Tuple [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        ) + (node.size() > 0
                 ? this->visitChildren(node)
                 : "");
    }

    auto ASTPrinter::operator()(syntax::decl::GenericIdentifier const& node) -> Str {
        return std::format(
            "decl::GenericIdentifier [@{}:{}] with name = '{}'",
            node.line(), node.column(), node.name()
        );
    }

    auto ASTPrinter::operator()(syntax::decl::GenericParameters const& node) -> Str {
        return std::format(
            "decl::GenericParameters [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        ) + (node.size() > 0
                 ? this->visitChildren(node)
                 : "");
    }

    auto ASTPrinter::operator()(syntax::stmt::Return const& node) -> Str {
        return std::format(
            "stmt::Return [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Decl const& node) -> Str {
        return std::format(
            "stmt::Decl [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Expression const& node) -> Str {
        return std::format(
            "stmt::Expression [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Assign const& node) -> Str {
        return std::format(
            "stmt::Assign [@{}:{}] with op = '{}'",
            node.line(), node.column(), node.op().str()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Conditional const& node) -> Str {
        return std::format(
            "stmt::Conditional [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Block const& node) -> Str {
        return std::format(
            "stmt::Block [@{}:{}] with size = {}",
            node.line(), node.column(), node.size()
        ) + (
            node.size() == 0
                ? ""
                : this->visitChildren(node)
        );
    }

    auto ASTPrinter::operator()(syntax::stmt::Defer const& node) -> Str {
        return std::format(
            "stmt::Defer [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Loop const& node) -> Str {
        return std::format(
            "stmt::Loop [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::MatchCase const& node) -> Str {
        return std::format(
            "stmt::MatchCase [@{}:{}]",
            node.line(), node.column()
        ) + "\n" + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::stmt::Match const& node) -> Str {
        return std::format(
            "stmt::Match [@{}:{}]",
            node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::global::ModuleDecl const& node) -> Str {
        return std::format(
            "global::ModuleDecl [@{}:{}]", node.line(), node.column()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::global::ImportDecl const& node) -> Str {
        return std::format(
            "global::ImportDecl [@{}:{}]", node.line(), node.column()
        ) + "\n" + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(
        syntax::global::FunctionPrototype const& node
    ) -> Str {
        return std::format(
            "global::FunctionPrototype [@{}:{}] with name = '{}'",
            node.line(), node.column(), node.name()
        ) + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::global::Function const& node) -> Str {
        return std::format(
            "global::Function [@{}:{}] with visibility = '{}'",
            node.line(), node.column(), node.visibility().str()
        ) + "\n" + this->visitChildren(node);
    }

    auto ASTPrinter::operator()(syntax::Empty const&) -> Str {
        return Str{empty};
    }

    auto ASTPrinter::operator()(syntax::RequiredButMissing const&) -> Str {
        return Str{required};
    }

    auto ASTPrinter::operator()(syntax::TranslationUnit const& node) -> Str {
        return SyntaxTreeVisitor::visitChildren(node) | rvJoinWithEl;
    }

    auto ASTPrinter::visitChildren(auto node) -> Str {
        Str const depthPrefix = [](szt const d) constexpr static {
            if (d == 0) {
                return prefix;
            }
            return (Vec(d, space) | rv::join | rng::to<Str>()) + prefix;
        }(m_depth++);

        auto result = SyntaxTreeVisitor::visitChildren(node)
            | rv::transform([depthPrefix](Str const& child) {
                return depthPrefix + child;
            })
            | rvJoinWithEl;
        m_depth = std::max(1ul, m_depth) - 1;
        return result.empty() ? "" : std::format("\n{}", std::move(result));
    }
}
