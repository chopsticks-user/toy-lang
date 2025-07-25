#include "nodes.hpp"

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Float::Float(f64 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Boolean::Boolean(b8 const value, Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Identifier::Identifier(
            Vec<Str> path, token::EToken const type, Coords coords
        ) : NodeBase{{}, std::move(coords)}, m_path{std::move(path)},
            m_type{type} {}

        Array::Array(Vec<Node> elements, Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

        Tuple::Tuple(Vec<Node> elements, Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

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
            Node object, Node field, Coords coords
        ): NodeBase{
            {std::move(object), std::move(field)},
            std::move(coords)
        } {}

        auto Access::object() const noexcept -> Node {
            return childAt(0);
        }

        auto Access::field() const noexcept -> Node {
            return childAt(1);
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
    }

    namespace type {
        Identifier::Identifier(
            Vec<Str> path, b8 const fundamental, Coords coords
        ): NodeBase{{}, std::move(coords)},
           m_path{std::move(path)}, m_fundamental{fundamental} {}

        auto Identifier::path() const noexcept -> Str {
            if (m_path.empty()) {
                return "";
            }

            Str pathStr = m_path.front();

            if (!imported()) {
                return pathStr;
            }

            for (Str s : m_path | rv::drop(1)) {
                pathStr += "::"s + s;
            }

            return pathStr;
        }

        Array::Array(
            Node type, Vec<Opt<szt>> sizes, Coords coords
        ): NodeBase{{std::move(type)}, std::move(coords)},
           m_sizes{std::move(sizes)} {}

        auto Array::type() const -> Node {
            return firstChild();
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
}
