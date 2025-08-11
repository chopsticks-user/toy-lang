// ReSharper disable CppMemberFunctionMayBeConst
#include "pretty_printer.hpp"

// #include <format>

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
        auto const fragments = node.fragments();
        return std::format(
            "\"{}{}\"",
            rv::zip(
                fragments | rv::take(fragments.size() - 1), visitChildren(node)
            ) | rv::transform([](std::tuple<Str, Str> const& t) {
                auto [fragment, placeholder] = t;
                return std::format(
                    "{}{{{}}}", std::move(fragment), std::move(placeholder)
                );
            }) | rv::join | rng::to<Str>(), fragments.back()
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
        return node.constant() ? node.path() : "$" + node.path();
    }

    auto PrettyPrint::operator()(syntax::type::Infer const& node) -> Str {
        return std::format("[[ {} ]]", visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::type::Array const& node) -> Str {
        return visitChildren(node) | rvJoin;
    }

    auto PrettyPrint::operator()(syntax::type::Tuple const& node) -> Str {
        return std::format("({})", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::type::Function const& node) -> Str {
        return std::format(
            "({})", visitChildren(node) | rv::join_with(" -> "sv)
            | rng::to<Str>()
        );
    }

    auto PrettyPrint::operator()(syntax::type::GenericArguments const& node) -> Str {
        return std::format("<{}>", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::type::Generic const& node) -> Str {
        return visitChildren(node) | rvJoin;
    }

    auto PrettyPrint::operator()(syntax::type::Binary const& node) -> Str {
        auto children = visitChildren(node);
        return std::format(
            "({} {} {})", std::move(children.front()), node.op().str(),
            visitChildren(node).back()
        );
    }

    auto PrettyPrint::operator()(syntax::decl::Identifier const& node) -> Str {
        return std::format("{}: {}", node.name(), visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::decl::Tuple const& node) -> Str {
        return std::format("({})", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::decl::GenericIdentifier const& node) -> Str {
        return Str{node.name()};
    }

    auto PrettyPrint::operator()(syntax::decl::GenericParameters const& node) -> Str {
        return std::format("<{}>", visitChildren(node) | rvJoinWithComma);
    }

    auto PrettyPrint::operator()(syntax::stmt::Decl const& node) -> Str {
        auto children = visitChildren(node);
        return std::format(
            "{}{} = {};",
            depthPrefix(), std::move(children.front()),
            std::move(children.back())
        );
    }

    auto PrettyPrint::operator()(syntax::stmt::Return const& node) -> Str {
        return std::format("{}return {};", depthPrefix(),
                           visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::stmt::Expression const& node) -> Str {
        return std::format("{}{};", depthPrefix(),
                           visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::stmt::Assign const& node) -> Str {
        auto children = visitChildren(node);
        return std::format("{}{} {} {};", depthPrefix(),
                           std::move(children.front()), node.op().str(),
                           std::move(children.back())
        );
    }

    auto PrettyPrint::operator()(syntax::stmt::Conditional const& node) -> Str {
        auto children = visitChildren(node);
        return std::format("{}{} => {}", depthPrefix(),
                           std::move(children.front()),
                           std::move(children.back()) | rvWithoutIdent);
    }

    auto PrettyPrint::operator()(syntax::stmt::Block const& node) -> Str {
        // todo: fix strings appended every scope
        auto prefix = depthPrefix();
        ++m_depth;
        auto children = visitChildren(node)
            | rv::join_with('\n') | rng::to<Str>();
        --m_depth;
        return children.empty()
                   ? std::format("{}{{}}", prefix)
                   : std::format(
                       "{}{{\n{}\n{}}}",
                       prefix, std::move(children), prefix
                   );
    }

    auto PrettyPrint::operator()(syntax::stmt::Defer const& node) -> Str {
        return std::format("{}defer {}", depthPrefix(),
                           visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::stmt::Loop const& node) -> Str {
        auto children = visitChildren(node);
        return std::format(
            "for {} in {} {}",
            std::move(children[0]), std::move(children[1]),
            std::move(children[2])
        );
    }

    auto PrettyPrint::operator()(syntax::stmt::MatchCase const& node) -> Str {
        auto children = visitChildren(node);
        return std::format("{}{} when {} => {}", depthPrefix(),
                           std::move(children[0]), std::move(children[1]),
                           std::move(children[2]) | rvWithoutIdent);
    }

    auto PrettyPrint::operator()(syntax::stmt::Match const& node) -> Str {
        ++m_depth;
        auto children = visitChildren(node);
        auto expr = children.front();
        auto cases = children | rv::as_rvalue | rv::drop(1);
        cases.back() = std::format("{}_ => {}", depthPrefix(),
                                   cases.back() | rvWithoutIdent);
        --m_depth;
        return std::format("{}match {} {{\n{}\n}}", depthPrefix(),
                           std::move(expr), cases | rv::join_with('\n')
                           | rng::to<Str>());
    }

    auto PrettyPrint::operator()(syntax::global::ModuleDecl const& node) -> Str {
        return std::format("module {};", visitChildren(node).front());
    }

    auto PrettyPrint::operator()(syntax::global::ImportDecl const& node) -> Str {
        auto children = visitChildren(node);
        if (children.back() == empty) {
            return std::format("import {};", children.front());
        }
        return std::format(
            "import {} = {};", children.front(), children.back()
        );
    }

    auto PrettyPrint::operator()(syntax::global::ImportDeclGroup const& node) -> Str {
        return std::format(
            "{}", visitChildren(node) | rv::join_with('\n') | rng::to<Str>()
        );
    }

    auto PrettyPrint::operator()(syntax::Empty const&) -> Str {
        return Str{empty};
    }

    auto PrettyPrint::operator()(syntax::RequiredButMissing const&) -> Str {
        return Str{required};
    }

    auto PrettyPrint::operator()(syntax::TranslationUnit const& node) -> Str {
        auto children = visitChildren(node);
        auto moduleDecl = children.front();
        auto importDeclGroup = children[1];

        if (moduleDecl == required) {
            return "";
        }
        if (children.size() < 3) {
            if (importDeclGroup == empty) {
                return std::format("{}", std::move(moduleDecl));
            }
            return std::format(
                "{}\n\n{}", std::move(moduleDecl), std::move(importDeclGroup)
            );
        }

        auto definitions = children | rv::as_rvalue | rv::drop(2)
            | rv::join_with("\n\n"sv) | rng::to<Str>();
        if (importDeclGroup == empty) {
            return std::format(
                "{}\n\n{}", std::move(moduleDecl), std::move(definitions)
            );
        }
        return std::format(
            "{}\n\n{}\n\n{}", std::move(moduleDecl),
            std::move(importDeclGroup), std::move(definitions)
        );
    }
}
