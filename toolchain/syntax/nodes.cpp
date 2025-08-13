export module syntax:nodes;

import lexeme;
import core;

export namespace tlc::syntax {
    namespace expr {
        struct Integer;
        struct Float;
        struct Boolean;
        struct Identifier;
        struct String;
        struct Array;
        struct Tuple;
        struct RecordEntry;
        struct Record;
        struct Try;
        struct FnApp;
        struct Subscript;
        struct Prefix;
        struct Binary;
    }

    namespace type {
        struct Identifier;
        struct Array;
        struct Tuple;
        struct Function;
        struct Infer;
        struct GenericArguments;
        struct Generic;
        struct Binary;
    }

    namespace decl {
        struct Identifier;
        struct Tuple;
        struct GenericIdentifier;
        struct GenericParameters;
    }

    namespace stmt {
        struct Decl;
        struct Return;
        struct Defer;
        struct MatchCase;
        struct Match;
        struct Loop;
        struct Conditional;
        struct Block;
        struct Assign;
        struct Expression;
    }

    namespace global {
        struct ModuleDecl;
        struct ImportDecl;
        struct ImportDeclGroup;
        struct FunctionPrototype;
        struct Function;
    }

    using Empty = std::monostate;
    struct RequiredButMissing;
    struct TranslationUnit;

    using Node = Poly<
        expr::Integer, expr::Float, expr::Boolean, expr::Identifier, expr::Array,
        expr::Tuple, expr::String, expr::FnApp, expr::Subscript, expr::Prefix,
        expr::Binary, expr::RecordEntry, expr::Record, expr::Try,

        type::Identifier, type::Array, type::Tuple, type::Function, type::Infer,
        type::GenericArguments, type::Generic, type::Binary,

        decl::Identifier, decl::Tuple, decl::GenericIdentifier,
        decl::GenericParameters,

        stmt::Decl, stmt::Return, stmt::Defer, stmt::Loop, stmt::Match,
        stmt::MatchCase, stmt::Conditional, stmt::Block, stmt::Assign,
        stmt::Expression,

        global::ModuleDecl, global::ImportDecl, global::FunctionPrototype,
        global::Function, global::ImportDeclGroup,

        RequiredButMissing, TranslationUnit
    >;
}

namespace tlc::syntax::detail {
    export class NodeBase {
    public:
        [[nodiscard]] auto children() const noexcept -> Span<Node const>;

        [[nodiscard]] auto children() noexcept -> Span<Node>;

        [[nodiscard]] auto childAt(szt index) const -> Node const&;

        [[nodiscard]] auto childAt(szt index) -> Node&;

        [[nodiscard]] auto firstChild() const -> Node const&;

        [[nodiscard]] auto firstChild() -> Node&;

        [[nodiscard]] auto lastChild() const -> Node const&;

        [[nodiscard]] auto lastChild() -> Node&;

        [[nodiscard]] auto nChildren() const noexcept -> szt;

        [[nodiscard]] auto line() const noexcept -> szt {
            return m_location.line;
        }

        [[nodiscard]] auto column() const noexcept -> szt {
            return m_location.column;
        }

    protected:
        NodeBase(Vec<Node> children, Location coords) noexcept;

    private:
        Vec<Node> m_children;
        Location m_location;
    };

    class IdentifierBase {
    public:
        explicit IdentifierBase(Vec<Str> path)
            : m_path{std::move(path)} {}

        [[nodiscard]] auto name() const noexcept -> Str {
            return m_path.empty() ? "" : m_path.back();
        }

        [[nodiscard]] auto path() const noexcept -> Str;

        [[nodiscard]] auto imported() const noexcept -> bool {
            return m_path.size() > 1;
        }

        [[nodiscard]] auto anonymous() const noexcept -> bool {
            return m_path.size() > 1;
        }

    protected:
        Vec<Str> m_path;
    };
}

namespace tlc::syntax {
    namespace expr {
        struct Integer final : detail::NodeBase {
            Integer(i64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> i64 {
                return m_value;
            }

        private:
            i64 m_value;
        };

        struct Float final : detail::NodeBase {
            Float(f64 value, Location location);

            [[nodiscard]] auto value() const noexcept -> double {
                return m_value;
            }

        private:
            f64 m_value;
        };

        struct Boolean final : detail::NodeBase {
            Boolean(b8 value, Location location);

            [[nodiscard]] auto value() const noexcept -> b8 {
                return m_value;
            }

        private:
            b8 m_value;
        };

        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(Vec<Str> path, Location location);
        };

        struct Array final : detail::NodeBase {
            Array(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> elements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct FnApp final : detail::NodeBase {
            FnApp(Node callee, Node args, Location location);
        };

        struct Subscript final : detail::NodeBase {
            Subscript(Node collection, Node subscript, Location location);
        };

        struct Prefix final : detail::NodeBase {
            Prefix(Node operand, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Binary final : detail::NodeBase {
            Binary(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct String final : detail::NodeBase {
            String(Vec<Str> fragments, Vec<Node> placeholders, Location location);

            [[nodiscard]] auto fragments() const noexcept -> Span<Str const> {
                return m_fragments;
            }

            [[nodiscard]] auto nPlaceholders() const noexcept -> szt {
                return m_fragments.size() - 1;
            }

            [[nodiscard]] auto interpolated() const noexcept -> b8 {
                return nPlaceholders() > 0;
            }

        private:
            Vec<Str> m_fragments;
        };

        struct RecordEntry final : detail::NodeBase {
            RecordEntry(Str key, Node value, Location location);

            [[nodiscard]] auto key() const noexcept -> StrV {
                return m_key;
            }

        private:
            Str m_key;
        };

        struct Record final : detail::NodeBase {
            Record(Node type, Vec<Node> entries, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Try final : detail::NodeBase {
            Try(Node expr, Location location);
        };
    }

    namespace type {
        struct Identifier : detail::NodeBase, detail::IdentifierBase {
            Identifier(
                b8 constant, Vec<Str> path, b8 fundamental, Location location
            );

            [[nodiscard]] auto fundamental() const noexcept -> bool {
                return m_fundamental;
            }

            [[nodiscard]] auto constant() const noexcept -> bool {
                return m_constant;
            }

        private:
            b8 m_fundamental;
            b8 m_constant;
        };

        struct Array final : detail::NodeBase {
            Array(Node type, Node sizes, Location location);
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> types, Location location);

            [[nodiscard]] auto size() const -> szt;
        };

        struct Function final : detail::NodeBase {
            Function(Node args, Node result, Location location);
        };

        struct Infer final : detail::NodeBase {
            Infer(Node expr, Location location);

            [[nodiscard]] auto expr() const noexcept -> Node;
        };

        struct GenericArguments final : detail::NodeBase {
            GenericArguments(Vec<Node> args, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Generic final : detail::NodeBase {
            Generic(Node type, Node args, Location location);
        };

        struct Binary final : detail::NodeBase {
            Binary(Node lhs, lexeme::Lexeme op, Node rhs, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };
    }

    namespace decl {
        struct Identifier final : detail::NodeBase {
            Identifier(Str name, Node type, Location location);

            [[nodiscard]] auto name() const noexcept -> Str const& {
                return m_name;
            }

            [[nodiscard]] auto inferred() const noexcept -> b8;

        private:
            Str m_name;
        };

        struct Tuple final : detail::NodeBase {
            Tuple(Vec<Node> decls, Location location);

            [[nodiscard]] auto decl(szt index) const -> Node;

            [[nodiscard]] auto size() const -> szt;
        };

        struct GenericIdentifier final : detail::NodeBase {
            GenericIdentifier(Str name, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct GenericParameters final : detail::NodeBase {
            GenericParameters(Vec<Node> params, Location location);

            [[nodiscard]] auto size() const -> szt;
        };
    }

    namespace stmt {
        struct Decl final : detail::NodeBase {
            Decl(Node decl, Node initializer, Location location);

            [[nodiscard]] auto defaultInitialized() const -> bool;
        };

        struct Return final : detail::NodeBase {
            Return(Node expr, Location location);
        };

        struct Defer final : detail::NodeBase {
            Defer(Node stmt, Location location);
        };

        struct MatchCase final : detail::NodeBase {
            MatchCase(Node value, Node cond, Node stmt, Location location);
        };

        struct Match final : detail::NodeBase {
            Match(Node expr, Vec<Node> cases, Node defaultStmt, Location location);
        };


        struct Loop final : detail::NodeBase {
            Loop(Node decl, Node range, Node body, Location location);
        };

        struct Conditional final : detail::NodeBase {
            Conditional(Node cond, Node then, Location location);
        };

        struct Block final : detail::NodeBase {
            Block(Vec<Node> statements, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct Assign final : detail::NodeBase {
            Assign(Node lhs, Node rhs, lexeme::Lexeme op, Location location);

            [[nodiscard]] auto op() const noexcept -> lexeme::Lexeme {
                return m_op;
            }

        private:
            lexeme::Lexeme m_op;
        };

        struct Expression final : detail::NodeBase {
            Expression(Node expr, Location location);
        };
    }

    namespace global {
        struct ModuleDecl final : detail::NodeBase {
            ModuleDecl(Node path, Location location);
        };

        struct ImportDeclGroup final : detail::NodeBase {
            ImportDeclGroup(Vec<Node> imports, Location location);

            [[nodiscard]] auto size() const noexcept -> szt;
        };

        struct ImportDecl final : detail::NodeBase {
            ImportDecl(Node alias, Node path, Location location);
        };

        struct FunctionPrototype final : detail::NodeBase {
            FunctionPrototype(Node genericDecl, Str name, Node paramsDecl,
                              Node returnsDecl, Location location);

            [[nodiscard]] auto name() const noexcept -> StrV {
                return m_name;
            }

        private:
            Str m_name;
        };

        struct Function final : detail::NodeBase {
            Function(lexeme::Lexeme visibility, Node prototype, Node body,
                     Location location);

            [[nodiscard]] auto visibility() const noexcept
                -> lexeme::Lexeme const& {
                return m_visibility;
            }

        private:
            lexeme::Lexeme m_visibility;
        };
    }

    struct RequiredButMissing final : detail::NodeBase {
        explicit RequiredButMissing();
    };

    struct TranslationUnit final : detail::NodeBase {
        TranslationUnit(
            fs::path sourcePath, Node moduleDecl,
            Node importDeclGroup, Vec<Node> definitions
        );

        [[nodiscard]] auto sourcePath() const noexcept -> fs::path const& {
            return m_sourcePath;
        }

        [[nodiscard]] auto size() const noexcept -> szt;

    private:
        fs::path m_sourcePath;
    };
}

namespace tlc::syntax::detail {
    NodeBase::NodeBase(Vec<Node> children, Location coords) noexcept
        : m_children(std::move(children)), m_location(std::move(coords)) {}

    auto NodeBase::children() const noexcept -> Span<Node const> {
        return m_children;
    }

    auto NodeBase::children() noexcept -> Span<Node> {
        return m_children;
    }

    auto NodeBase::childAt(const szt index) const -> const Node& {
        return m_children.at(index);
    }

    auto NodeBase::firstChild() const -> Node const& {
        return m_children.front();
    }

    auto NodeBase::lastChild() const -> Node const& {
        return m_children.back();
    }

    auto NodeBase::childAt(szt const index) -> Node& {
        return m_children.at(index);
    }

    auto NodeBase::firstChild() -> Node& {
        return m_children.front();
    }

    auto NodeBase::lastChild() -> Node& {
        return m_children.back();
    }

    auto NodeBase::nChildren() const noexcept -> szt {
        return m_children.size();
    }

    auto IdentifierBase::path() const noexcept -> Str {
        if (m_path.empty()) {
            return "";
        }

        Str pathStr = m_path.front();

        if (!imported()) {
            return pathStr;
        }

        for (StrV s : m_path | rv::drop(1)) {
            pathStr += "."s + Str(s);
        }

        return pathStr;
    }
}

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value, Location const location)
            : NodeBase{{}, location}, m_value{value} {}

        Float::Float(f64 const value, Location const location)
            : NodeBase{{}, location}, m_value{value} {}

        Boolean::Boolean(b8 const value, Location const location)
            : NodeBase{{}, location}, m_value{value} {}

        Identifier::Identifier(
            Vec<Str> path, Location const location
        ) : NodeBase{{}, location},
            IdentifierBase{std::move(path)} {}

        Array::Array(Vec<Node> elements, Location const location)
            : NodeBase{std::move(elements), location} {}

        auto Array::size() const noexcept -> szt {
            return nChildren();
        }

        Tuple::Tuple(Vec<Node> elements, Location const location)
            : NodeBase{std::move(elements), location} {}

        auto Tuple::size() const noexcept -> szt {
            return nChildren();
        }

        FnApp::FnApp(Node callee, Node args, Location const location)
            : NodeBase{
                {std::move(callee), std::move(args)},
                location
            } {}

        Subscript::Subscript(
            Node collection, Node subscript, Location const location
        ) : NodeBase{
            {std::move(collection), std::move(subscript)},
            location
        } {}

        Prefix::Prefix(
            Node operand, lexeme::Lexeme op, Location const location
        ) : NodeBase{{std::move(operand)}, location},
            m_op{std::move(op)} {}

        Binary::Binary(
            Node lhs, Node rhs, lexeme::Lexeme op, Location const location
        ) : NodeBase{{std::move(lhs), std::move(rhs)}, location},
            m_op{std::move(op)} {}

        String::String(
            Vec<Str> fragments, Vec<Node> placeholders, Location const location
        )
            : NodeBase{std::move(placeholders), location},
              m_fragments{std::move(fragments)} {}

        RecordEntry::RecordEntry(Str key, Node value, Location const location)
            : NodeBase{{std::move(value)}, location},
              m_key{std::move(key)} {}

        Record::Record(Node type, Vec<Node> entries, Location const location)
            : NodeBase{
                {
                    [&] {
                        Vec<Node> nodes;
                        nodes.reserve(entries.size() + 1);
                        nodes.push_back(std::move(type));
                        nodes.append_range(std::move(entries));
                        return nodes;
                    }()
                },
                location
            } {}

        auto Record::size() const noexcept -> szt {
            return nChildren() - 1;
        }

        Try::Try(Node expr, Location const location)
            : NodeBase{{std::move(expr)}, location} {}
    }

    namespace type {
        Identifier::Identifier(
            b8 const constant, Vec<Str> path, b8 const fundamental,
            Location const location
        ) : NodeBase{{}, location}, IdentifierBase{std::move(path)},
            m_fundamental{fundamental}, m_constant{constant} {}

        Array::Array(
            Node type, Node sizes, Location const location
        ) : NodeBase{
            [&] {
                Vec<Node> nodes;
                nodes.reserve(2);
                nodes.push_back(std::move(type));
                nodes.push_back(std::move(sizes));
                return nodes;
            }(),
            location
        } {}

        Tuple::Tuple(Vec<Node> types, Location const location)
            : NodeBase{std::move(types), location} {}

        auto Tuple::size() const -> szt {
            return nChildren();
        }

        Function::Function(Node args, Node result, Location const location)
            : NodeBase{{std::move(args), std::move(result)}, location} {}

        Infer::Infer(Node expr, Location const location)
            : NodeBase{{std::move(expr)}, location} {}

        auto Infer::expr() const noexcept -> Node {
            return firstChild();
        }

        GenericArguments::GenericArguments(
            Vec<Node> args, Location const location
        )
            : NodeBase{std::move(args), location} {}

        auto GenericArguments::size() const noexcept -> szt {
            return nChildren();
        }

        Generic::Generic(Node type, Node args, Location const location)
            : NodeBase{{std::move(type), std::move(args)}, location} {}

        Binary::Binary(Node lhs, lexeme::Lexeme op, Node rhs, Location location)
            : NodeBase{{std::move(lhs), std::move(rhs)}, std::move(location)},
              m_op{std::move(op)} {}
    }

    namespace decl {
        Identifier::Identifier(
            Str name, Node type, Location const location
        ) : NodeBase{{std::move(type)}, location},
            m_name{std::move(name)} {}


        auto Identifier::inferred() const noexcept -> b8 {
            return std::holds_alternative<std::monostate>(firstChild());
        }

        Tuple::Tuple(Vec<Node> decls, Location const location)
            : NodeBase{std::move(decls), location} {}

        auto Tuple::decl(szt const index) const -> Node {
            return childAt(index);
        }

        auto Tuple::size() const -> szt {
            return nChildren();
        }

        GenericIdentifier::GenericIdentifier(Str name, Location const location)
            : NodeBase{{}, location}, m_name{std::move(name)} {}

        GenericParameters::GenericParameters(
            Vec<Node> params, Location const location)
            : NodeBase{std::move(params), location} {}

        auto GenericParameters::size() const -> szt {
            return nChildren();
        }
    }

    stmt::Decl::Decl(Node decl, Node initializer, Location const location)
        : NodeBase{{std::move(decl), std::move(initializer)}, location} {}

    auto stmt::Decl::defaultInitialized() const -> bool {
        return std::holds_alternative<std::monostate>(lastChild());
    }

    stmt::Return::Return(Node expr, Location const location)
        : NodeBase{{std::move(expr)}, location} {}

    stmt::Defer::Defer(Node stmt, Location const location)
        : NodeBase{{std::move(stmt)}, location} {}

    stmt::MatchCase::MatchCase(Node value, Node cond, Node stmt, Location const location)
        : NodeBase{
            {std::move(value), std::move(cond), std::move(stmt)},
            location
        } {}

    stmt::Match::Match(Node expr, Vec<Node> cases, Node defaultStmt, Location const location)
        : NodeBase{
            {
                [&] {
                    Vec<Node> nodes;
                    nodes.reserve(cases.size() + 2);
                    nodes.push_back(std::move(expr));
                    nodes.append_range(std::move(cases));
                    nodes.push_back(std::move(defaultStmt));
                    return nodes;
                }()
            },
            location
        } {}

    stmt::Loop::Loop(Node decl, Node range, Node body, Location const location)
        : NodeBase{
            {std::move(decl), std::move(range), std::move(body)},
            location
        } {}

    stmt::Conditional::Conditional(Node cond, Node then, Location const location)
        : NodeBase{
            {std::move(cond), std::move(then)},
            location
        } {}

    stmt::Block::Block(Vec<Node> statements, Location const location)
        : NodeBase{std::move(statements), location} {}

    auto stmt::Block::size() const noexcept -> szt {
        return nChildren();
    }

    stmt::Assign::Assign(
        Node lhs, Node rhs, lexeme::Lexeme op, Location const location
    ) : NodeBase{{std::move(lhs), std::move(rhs)}, location},
        m_op{std::move(op)} {}

    stmt::Expression::Expression(Node expr, Location const location)
        : NodeBase{{std::move(expr)}, location} {}

    global::ModuleDecl::ModuleDecl(Node path, Location const location)
        : NodeBase{{std::move(path)}, location} {}

    global::ImportDeclGroup::ImportDeclGroup(
        Vec<Node> imports, Location location
    ) : NodeBase{std::move(imports), std::move(location)} {}

    auto global::ImportDeclGroup::size() const noexcept -> szt {
        return nChildren();
    }

    global::ImportDecl::ImportDecl(Node alias, Node path, Location const location)
        : NodeBase{{std::move(alias), std::move(path)}, location} {}

    global::FunctionPrototype::FunctionPrototype(
        Node genericDecl, Str name, Node paramsDecl,
        Node returnsDecl, Location const location
    ) : NodeBase{
            {
                std::move(genericDecl), std::move(paramsDecl),
                std::move(returnsDecl)
            },
            location
        },
        m_name{std::move(name)} {}

    global::Function::Function(
        lexeme::Lexeme visibility, Node prototype, Node body,
        Location const location
    ) : NodeBase{{std::move(prototype), std::move(body)}, location},
        m_visibility{std::move(visibility)} {}

    RequiredButMissing::RequiredButMissing()
        : NodeBase{{}, {}} {}

    TranslationUnit::TranslationUnit(
        fs::path sourcePath, Node moduleDecl,
        Node importDeclGroup, Vec<Node> definitions
    ) : NodeBase{
            [&] {
                Vec<Node> nodes;
                nodes.reserve(2 + definitions.size());
                nodes.push_back(std::move(moduleDecl));
                nodes.push_back(std::move(importDeclGroup));
                nodes.append_range(std::move(definitions));
                return nodes;
            }(),
            {}
        }, m_sourcePath{std::move(sourcePath)} {}

    auto TranslationUnit::size() const noexcept -> szt {
        return nChildren();
    }
}
