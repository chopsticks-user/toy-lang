#include "parse.hpp"

namespace tlc::parse {
    auto Parse::operator()(fs::path filepath, Vec<token::Token> tokens) -> syntax::Node {
        return Parse{std::move(filepath), std::move(tokens)}();
    }

    auto Parse::operator()() -> syntax::Node {
        return handleTranslationUnit().value_or(syntax::TranslationUnit{
            m_filepath, syntax::RequiredButMissing{},
            {}, {}
        });
    }
}
