#include "parse.hpp"

namespace tlc::parse {
    auto Parse::operator()(fs::path filepath, Vec<token::Token> tokens) -> syntax::Node {
        return Parse{std::move(filepath), std::move(tokens)}();
    }

    auto Parse::operator()() -> syntax::Node {
        return *handleTranslationUnit().or_else(
            [this](auto&& err) -> ParseResult {
                collect(err);
                return syntax::TranslationUnit{m_filepath, {}};
            }
        );
    }
}
