#include "nodes.hpp"

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Float::Float(f64 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Boolean::Boolean(b8 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Identifier::Identifier(
            Vec<Str> path, token::EToken type, token::Token::Coords coords
        ) : NodeBase{{}, std::move(coords)}, m_path(std::move(path)) {}

        Array::Array(Vec<Node> elements, token::Token::Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

        Tuple::Tuple(Vec<Node> elements, token::Token::Coords coords)
            : NodeBase{std::move(elements), std::move(coords)} {}

        FnApp::FnApp(Node callee, Node args, token::Token::Coords coords)
            : NodeBase{
                {std::move(callee), std::move(args)},
                std::move(coords)
            } {}

        Subscript::Subscript(
            Node collection, Node subscript, token::Token::Coords coords
        ): NodeBase{
            {std::move(collection), std::move(subscript)},
            std::move(coords)
        } {}

        Access::Access(
            Node object, Node field, token::Token::Coords coords
        ): NodeBase{
            {std::move(object), std::move(field)},
            std::move(coords)
        } {}

        Unary::Unary(
            Node operand, token::EToken const op, token::Token::Coords coords
        ): NodeBase{{std::move(operand)}, std::move(coords)},
           m_op{op} {}

        Binary::Binary(
            Node lhs, Node rhs, token::EToken const op, token::Token::Coords coords
        ) : NodeBase{{std::move(lhs), std::move(rhs)}, std::move(coords)},
            m_op{op} {}
    }
}
