#include "parse.hpp"

namespace tlc::parse {
    auto Parse::operator()(Vec<token::Token> tokens) -> syntax::Node {
        return Parse{std::move(tokens)}();
    }

    auto Parse::operator()() -> syntax::Node {
        return {};
    }

}
