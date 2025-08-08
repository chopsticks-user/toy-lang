#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleFunctionDef(token::Token const& visibility) -> ParseResult {
        return handleFunctionPrototype().and_then(
            [this, &visibility](syntax::Node&& prototype) -> ParseResult {
                auto body = handleBlockStmt();
                if (!body) {
                    return error({
                        .context = EParseErrorContext::Function,
                        .reason = EParseErrorReason::MissingBody,
                    });
                }
                return syntax::global::Function{
                    visibility.lexeme(), std::move(prototype),
                    std::move(*body), visibility.location()
                };
            }
        );
    }

    auto Parse::handleFunctionPrototype() -> ParseResult {
        if (!m_stream.match(lexeme::fn)) {
            return defaultError();
        }

        auto location = m_tracker.current();
        auto genericDecl = handleGenericParamsDecl().value_or({});
        Str name;
        if (m_stream.match(lexeme::identifier)) {
            name = m_stream.current().str();
        }
        else {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::FunctionPrototype,
                .reason = EParseErrorReason::MissingId,
            });
        }
        if (!m_stream.match(lexeme::colon)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::FunctionPrototype,
                .reason = EParseErrorReason::MissingDecl,
            });
        }

        if (!m_stream.match(lexeme::colon)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::FunctionPrototype,
                .reason = EParseErrorReason::MissingDecl,
            });
        }
        auto paramsDecl = *handleTupleDecl()
            .or_else([this](auto&& err) -> ParseResult {
                collect(err).collect({
                    .context = EParseErrorContext::FunctionPrototype,
                    .reason = EParseErrorReason::MissingDecl,
                });
                return {};
            });
        if (!m_stream.match(lexeme::minusGreater)) {
            collect({
                .location = m_tracker.current(),
                .context = EParseErrorContext::FunctionPrototype,
                .reason = EParseErrorReason::MissingSymbol,
            });
        }
        auto returnsDecl = *handleTupleDecl()
            .or_else([this](auto&& err) -> ParseResult {
                collect(err);
                return {};
            });

        return syntax::global::FunctionPrototype{
            std::move(genericDecl), std::move(name), std::move(paramsDecl),
            std::move(returnsDecl), std::move(location)
        };
    }
}
