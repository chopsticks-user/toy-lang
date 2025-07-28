#include "nodes.hpp"
#include "util.hpp"

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Float::Float(f64 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Boolean::Boolean(b8 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Identifier::Identifier(
            Vec<Str> path, Coords coords
        ) : NodeBase{{}, std::move(coords)},
            IdentifierBase{std::move(path)} {}

        Array::Array(Vec<Node> elements, Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

        auto Array::size() const noexcept -> szt {
            return nChildren();
        }

        Tuple::Tuple(Vec<Node> elements, Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

        auto Tuple::size() const noexcept -> szt {
            return nChildren();
        }

        FnApp::FnApp(Node callee, Node args, Coords coords)
            : NodeBase{
                {std::move(callee), std::move(args)},
                std::move(coords)
            } {}

        auto FnApp::callee() const noexcept -> Node {
            return childAt(0);
        }

        auto FnApp::args() const noexcept -> Node {
            return childAt(1);
        }

        Subscript::Subscript(
            Node collection, Node subscript, Coords coords
        ): NodeBase{
            {std::move(collection), std::move(subscript)},
            std::move(coords)
        } {}

        auto Subscript::collection() const noexcept -> Node {
            return childAt(0);
        }

        auto Subscript::subscript() const noexcept -> Node {
            return childAt(1);
        }

        Access::Access(
            Node object, Str field, Coords coords
        ): NodeBase{
               {std::move(object)},
               std::move(coords)
           }, m_field{std::move(field)} {}

        auto Access::object() const noexcept -> Node {
            return childAt(0);
        }

        auto Access::field() const noexcept -> Str {
            return m_field;
        }

        Prefix::Prefix(
            Node operand, token::EToken const op, Coords coords
        ): NodeBase{{std::move(operand)}, std::move(coords)},
           m_op{op} {}

        auto Prefix::operand() const noexcept -> Node {
            return firstChild();
        }

        Binary::Binary(
            Node lhs, Node rhs, token::EToken const op, Coords coords
        ) : NodeBase{{std::move(lhs), std::move(rhs)}, std::move(coords)},
            m_op{op} {}

        auto Binary::left() const noexcept -> Node {
            return firstChild();
        }

        auto Binary::right() const noexcept -> Node {
            return lastChild();
        }

        Record::Record(Node of, Vec<Pair<Str, Node>> entries, Coords coords)
            : NodeBase{
                  [&] {
                      Vec<Node> nodes;
                      nodes.reserve(entries.size() + 1);
                      // todo: concat
                      nodes.push_back(std::move(of));
                      nodes.append_range(
                          entries | rv::transform(
                              [](auto const& entry) {
                                  return entry.second;
                              }
                          )
                      );
                      return nodes;
                  }(),
                  std::move(coords)
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
            return nChildren() - 1;
        }

        auto Record::of() const noexcept -> Node {
            return firstChild();
        }

        auto Record::value(szt const index) const noexcept -> Node const& {
            return childAt(index + 1);
        }
    }

    namespace type {
        Identifier::Identifier(
            Vec<Str> path, b8 const fundamental, Coords coords
        ): NodeBase{{}, std::move(coords)},
           IdentifierBase{std::move(path)}, m_fundamental{fundamental} {}

        Array::Array(
            Node type, Vec<Node> sizes, Coords coords
        ): NodeBase{
            [&] {
                Vec<Node> nodes;
                nodes.reserve(sizes.size() + 1);
                nodes.emplace_back(type);
                nodes.append_range(std::move(sizes));
                return nodes;
            }(),
            std::move(coords)
        } {}

        auto Array::type() const noexcept -> Node const& {
            return firstChild();
        }

        auto Array::size(szt const dimIndex) const noexcept -> Node const& {
            return childAt(dimIndex + 1);
        }

        auto Array::dim() const noexcept -> size_t {
            return nChildren() - 1;
        }

        auto Array::fixed(szt const dimIndex) const -> bool {
            return !isEmptyNode(size(dimIndex));
        }

        Tuple::Tuple(Vec<Node> types, Coords coords)
            : NodeBase{std::move(types), std::move(coords)} {}

        auto Tuple::type(szt const index) const -> Node {
            return childAt(index);
        }

        auto Tuple::size() const -> szt {
            return nChildren();
        }

        Function::Function(Node args, Node result, Coords coords)
            : NodeBase{{std::move(args), std::move(result)}, std::move(coords)} {}

        auto Function::args() const noexcept -> Node {
            return firstChild();
        }

        auto Function::result() const noexcept -> Node {
            return lastChild();
        }

        Infer::Infer(Node expr, Coords coords)
            : NodeBase{{std::move(expr)}, std::move(coords)} {}

        auto Infer::expr() const noexcept -> Node {
            return firstChild();
        }

        Sum::Sum(Vec<Node> types, Coords coords)
            : NodeBase{std::move(types), std::move(coords)} {}

        auto Sum::type(szt const index) const -> Node {
            return childAt(index);
        }

        Product::Product(Vec<Node> types, Coords coords)
            : NodeBase{std::move(types), std::move(coords)} {}

        auto Product::type(szt const index) const -> Node {
            return childAt(index);
        }
    }

    namespace decl {
        Identifier::Identifier(
            b8 const constant, Node identifier, Node type, Coords coords
        ) : NodeBase{{std::move(identifier), std::move(type)}, std::move(coords)},
            m_constant{constant} {}

        auto Identifier::identifier() const noexcept -> Node const& {
            return firstChild();
        }

        auto Identifier::type() const noexcept -> Node const& {
            return lastChild();
        }

        Tuple::Tuple(Vec<Node> decls, Coords coords)
            : NodeBase{std::move(decls), std::move(coords)} {}

        auto Tuple::decl(szt const index) const -> Node {
            return childAt(index);
        }

        auto Tuple::size() const -> szt {
            return nChildren();
        }
    }

    stmt::Yield::Yield(Node expr, Coords coords)
        : NodeBase{{std::move(expr)}, std::move(coords)} {}

    auto stmt::Yield::expr() const noexcept -> Node const& {
        return firstChild();
    }
}
