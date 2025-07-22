#include "nodes.hpp"

namespace tlc::syntax {
    namespace expr {
        Integer::Integer(i64 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Float::Float(f64 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        Boolean::Boolean(b8 const value, token::Token::Coords coords)
            : NodeBase{{}, std::move(coords)}, m_value{value} {}

        IdentifierBase::IdentifierBase(
            Vec<Str> path, token::Token::Coords coords
        ) : NodeBase{{}, std::move(coords)}, m_path(std::move(path)) {}

        VarId::VarId(Vec<Str> path, token::Token::Coords coords)
            : IdentifierBase{std::move(path), std::move(coords)} {}

        TypeId::TypeId(Vec<Str> path, token::Token::Coords coords)
            : IdentifierBase{std::move(path), std::move(coords)} {}
    }
}
