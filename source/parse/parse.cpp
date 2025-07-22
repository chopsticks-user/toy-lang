#include "parse.hpp"

namespace tlc::parse {
    auto Parser::operator()(Vec<token::Token> tokens) -> syntax::Node {
        return Parser{std::move(tokens)}();
    }

    auto Parser::operator()() -> syntax::Node {
        return {};
    }

}
