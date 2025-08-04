#include "nodes.hpp"
#include "util.hpp"

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
        ): NodeBase{
            {std::move(collection), std::move(subscript)},
            location
        } {}

        Access::Access(
            Node object, Str field, Location const location
        ): NodeBase{
               {std::move(object)},
               location
           }, m_field{std::move(field)} {}

        Prefix::Prefix(
            Node operand, lexeme::Lexeme op, Location const location
        ): NodeBase{{std::move(operand)}, location},
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
        ): NodeBase{{}, location}, IdentifierBase{std::move(path)},
           m_fundamental{fundamental}, m_constant{constant} {}

        Array::Array(
            Node type, Vec<Node> sizes, Location const location
        ): NodeBase{
            [&] {
                Vec<Node> nodes;
                nodes.reserve(sizes.size() + 1);
                nodes.emplace_back(type);
                nodes.append_range(std::move(sizes));
                return nodes;
            }(),
            location
        } {}

        auto Array::nDims() const noexcept -> szt {
            return nChildren() - 1;
        }

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
    }

    namespace decl {
        Identifier::Identifier(
            Str name, Node type, Location const location
        ) : NodeBase{{std::move(type)}, location},
            m_name{std::move(name)} {}


        auto Identifier::inferred() const noexcept -> b8 {
            return isEmptyNode(firstChild());
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
        return isEmptyNode(lastChild());
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
    ): NodeBase{{std::move(lhs), std::move(rhs)}, location},
       m_op{std::move(op)} {}

    stmt::Expression::Expression(Node expr, Location const location)
        : NodeBase{{std::move(expr)}, location} {}
}
