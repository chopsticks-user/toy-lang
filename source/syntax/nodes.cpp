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

        auto FnApp::callee() const noexcept -> Node {
            return childAt(0);
        }

        auto FnApp::args() const noexcept -> Node {
            return childAt(1);
        }

        Subscript::Subscript(
            Node collection, Node subscript, Location const location
        ): NodeBase{
            {std::move(collection), std::move(subscript)},
            location
        } {}

        auto Subscript::collection() const noexcept -> Node {
            return childAt(0);
        }

        auto Subscript::subscript() const noexcept -> Node {
            return childAt(1);
        }

        Access::Access(
            Node object, Str field, Location const location
        ): NodeBase{
               {std::move(object)},
               location
           }, m_field{std::move(field)} {}

        auto Access::object() const noexcept -> Node {
            return childAt(0);
        }

        auto Access::field() const noexcept -> Str {
            return m_field;
        }

        Prefix::Prefix(
            Node operand, lexeme::Lexeme op, Location const location
        ): NodeBase{{std::move(operand)}, location},
           m_op{std::move(op)} {}

        auto Prefix::operand() const noexcept -> Node {
            return firstChild();
        }

        Binary::Binary(
            Node lhs, Node rhs, lexeme::Lexeme op, Location const location
        ) : NodeBase{{std::move(lhs), std::move(rhs)}, location},
            m_op{std::move(op)} {}

        auto Binary::left() const noexcept -> Node {
            return firstChild();
        }

        auto Binary::right() const noexcept -> Node {
            return lastChild();
        }

        String::String(Vec<Str> fragments, Vec<Node> placeholders, Location location)
            : NodeBase{std::move(placeholders), location},
              m_fragments{std::move(fragments)} {}

        Record::Record(Node type, Vec<Pair<Str, Node>> entries, Location const location)
            : NodeBase{
                  [&] {
                      Vec<Node> nodes;
                      nodes.reserve(entries.size() + 1);
                      // todo: concat
                      nodes.push_back(std::move(type));
                      nodes.append_range(
                          entries | rv::transform(
                              [](auto const& entry) {
                                  return entry.second;
                              }
                          )
                      );
                      return nodes;
                  }(),
                  location
              }, m_keys{
                  [&] {
                      return entries | rv::transform(
                          [](auto const& entry) {
                              return entry.first;
                          }
                      ) | rng::to<Vec<Str>>();
                  }()
              } {}

        auto Record::size() const noexcept -> szt {
            return m_keys.size() - 1;
        }

        auto Record::type() const noexcept -> Node {
            return firstChild();
        }

        auto Record::value(szt const index) const noexcept -> Node const& {
            return childAt(index + 1);
        }
    }

    namespace type {
        Identifier::Identifier(
            Vec<Str> path, b8 const fundamental, Location const location
        ): NodeBase{{}, location},
           IdentifierBase{std::move(path)}, m_fundamental{fundamental} {}

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

        auto Array::type() const noexcept -> Node const& {
            return firstChild();
        }

        auto Array::size(szt const dimIndex) const noexcept -> Node const& {
            return childAt(dimIndex + 1);
        }

        auto Array::nDims() const noexcept -> size_t {
            return nChildren() - 1;
        }

        auto Array::fixed(szt const dimIndex) const -> bool {
            return !isEmptyNode(size(dimIndex));
        }

        Tuple::Tuple(Vec<Node> types, Location const location)
            : NodeBase{std::move(types), location} {}

        auto Tuple::type(szt const index) const -> Node {
            return childAt(index);
        }

        auto Tuple::size() const -> szt {
            return nChildren();
        }

        Function::Function(Node args, Node result, Location const location)
            : NodeBase{{std::move(args), std::move(result)}, location} {}

        auto Function::args() const noexcept -> Node {
            return firstChild();
        }

        auto Function::result() const noexcept -> Node {
            return lastChild();
        }

        Infer::Infer(Node expr, Location const location)
            : NodeBase{{std::move(expr)}, location} {}

        auto Infer::expr() const noexcept -> Node {
            return firstChild();
        }

        Generic::Generic(Node type, Vec<Node> args, Location const location)
            : NodeBase{
                {
                    [&] {
                        Vec<Node> nodes;
                        nodes.reserve(args.size() + 1);
                        nodes.push_back(std::move(type));
                        nodes.append_range(std::move(args));
                        return nodes;
                    }()
                },
                location
            } {}

        Binary::Binary(Node lhs, Node rhs, lexeme::Lexeme op,
                       Location const location)
            : NodeBase{{std::move(lhs), std::move(rhs)}, location},
              m_op{std::move(op)} {}
    }

    namespace decl {
        Identifier::Identifier(
            b8 const constant, Str name, Node type, Location const location
        ) : NodeBase{{std::move(type)}, location},
            m_constant{constant}, m_name{std::move(name)} {}

        auto Identifier::type() const noexcept -> Node const& {
            return firstChild();
        }

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
    }

    stmt::Let::Let(Node decl, Node initializer, Location const location)
        : NodeBase{{std::move(decl), std::move(initializer)}, location} {}

    auto stmt::Let::decl() const noexcept -> Node const& {
        return firstChild();
    }

    auto stmt::Let::initializer() const noexcept -> Node const& {
        return lastChild();
    }

    auto stmt::Let::defaultInitialized() const -> bool {
        return isEmptyNode(lastChild());
    }

    stmt::Return::Return(Node expr, Location const location)
        : NodeBase{{std::move(expr)}, location} {}

    auto stmt::Return::expr() const noexcept -> Node const& {
        return firstChild();
    }

    stmt::Yield::Yield(Node expr, Location const location)
        : NodeBase{{std::move(expr)}, location} {}

    auto stmt::Yield::expr() const noexcept -> Node const& {
        return firstChild();
    }

    stmt::Preface::Preface(Node stmt, Location const location)
        : NodeBase{{std::move(stmt)}, location} {}

    auto stmt::Preface::stmt() const noexcept -> Node const& {
        return firstChild();
    }

    stmt::Defer::Defer(Node stmt, Location const location)
        : NodeBase{{std::move(stmt)}, location} {}

    auto stmt::Defer::stmt() const noexcept -> Node const& {
        return firstChild();
    }

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
