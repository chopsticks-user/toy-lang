#include "parse.hpp"

#include "lex/lex.hpp"

namespace tlc::parse {
    auto Parse::handleExpr(syntax::OpPrecedence const minP) -> ParseResult { // NOLINT(*-no-recursion)
        auto lhsContext = enter(Context::Expr);
        ParseResult lhs;

        if (lhsContext.stream().match(syntax::isPrefixOperator)) {
            lhsContext.to(Context::PrefixExpr);

            auto const op = lhsContext.stream().current().lexeme();
            lhs = handleExpr(
                    syntax::opPrecedence(
                        op, syntax::EOperator::Prefix
                    ))
                .and_then([&](auto const& node) -> ParseResult {
                    return syntax::expr::Prefix{node, op, lhsContext.location()};
                });
        }
        else {
            lhs = handlePrimaryExpr();
        }
        if (!lhs) {
            return Unexpected{lhs.error()};
        }

        auto rhsContext = enter(Context::Expr);
        while (true) {
            if (syntax::isPostfixStart(rhsContext.stream().peek().lexeme())) {
                if (auto const tuple = handleTupleExpr(); tuple) {
                    lhs = syntax::expr::FnApp{
                        *lhs, *tuple, lhsContext.location()
                    };
                }
                else if (auto const array = handleArrayExpr(); array) {
                    lhs = syntax::expr::Subscript{
                        *lhs, *array, lhsContext.location()
                    };
                }
            }
            else if (rhsContext.stream().match(syntax::isBinaryOperator)) {
                rhsContext.to(Context::BinaryExpr);

                auto const op = rhsContext.stream().current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );
                if (rhsContext.backtrackIf(p <= minP)) {
                    break;
                }

                auto rhs = handleExpr(
                    syntax::isLeftAssociative(op) ? p + 1 : p
                ).value_or(syntax::RequiredButMissing{});
                // rhsContext.emitIfNodeEmpty(rhs, EParseErrorReason::MissingExpr);
                lhs = syntax::expr::Binary{
                    *lhs, std::move(rhs), op, lhsContext.location()
                };
            }
            else {
                break;
            }
        }

        return lhs;
    }

    auto Parse::handlePrimaryExpr() -> ParseResult { // NOLINT(*-no-recursion)
        if (auto const result = handleTryExpr(); result) {
            return result;
        }
        if (auto const result = handleSingleTokenLiteral(); result) {
            return result;
        }
        if (auto const result = handleRecordExpr(); result) {
            return result;
        }
        if (auto const result = handleIdentifierLiteral(); result) {
            return result;
        }
        if (auto const result = handleString(); result) {
            return result;
        }
        if (auto const result = handleTupleExpr(); result) {
            return result;
        }
        if (auto const result = handleArrayExpr(); result) {
            return result;
        }
        return defaultError();
    }

    auto Parse::handleSingleTokenLiteral() -> ParseResult {
        static const HashMap<lexeme::Lexeme, i32> baseTable = {
            {lexeme::integer2Literal, 2}, {lexeme::integer8Literal, 8},
            {lexeme::integer10Literal, 10}, {lexeme::integer16Literal, 16},
        };

        auto context = enter(Context::LiteralExpr);

        if (!context.stream().match(
            lexeme::integer2Literal, lexeme::integer8Literal,
            lexeme::integer10Literal, lexeme::integer16Literal,
            lexeme::floatLiteral, lexeme::true_, lexeme::false_
        )) {
            return defaultError();
        }

        switch (auto token = context.stream().current();
            token.lexeme().type()) {
        case lexeme::Lexeme::FloatLiteral:
            return syntax::expr::Float{
                std::stod(token.str()),
                context.location()
            };
        case lexeme::Lexeme::True:
        case lexeme::Lexeme::False:
            return syntax::expr::Boolean{
                token.lexeme() == lexeme::true_,
                context.location()
            };
        default:
            return syntax::expr::Integer{
                std::stoll(
                    token.str(), nullptr,
                    baseTable.at(token.lexeme())
                ),
                context.location()
            };
        }
    }

    auto Parse::handleIdentifierLiteral() -> ParseResult {
        auto context = enter(Context::IdentifierExpr);

        if (context.stream().match(lexeme::anonymous)) {
            return syntax::expr::Identifier{
                {context.stream().current().str()}, context.location()
            };
        }

        Vec<Str> fragments;
        while (context.stream().match(lexeme::identifier)) {
            fragments.push_back(context.stream().current().str());
            if (!context.stream().match(lexeme::dot)) {
                break;
            }
        }
        if (fragments.empty()) {
            return defaultError();
        }

        return syntax::expr::Identifier{
            std::move(fragments), context.location()
        };
    }

    auto Parse::handleTupleExpr() -> ParseResult { // NOLINT(*-no-recursion)
        auto context = enter(Context::TupleExpr);

        if (!context.stream().match(lexeme::leftParen)) {
            return defaultError();
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::expr::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> elements;
        do {
            auto expr = handleExpr().value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(expr, Reason::MissingExpr);
            elements.push_back(std::move(expr));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, Reason::MissingEnclosingSymbol
        );
        return syntax::expr::Tuple{std::move(elements), context.location()};
    }

    auto Parse::handleArrayExpr() -> ParseResult { // NOLINT(*-no-recursion)
        auto context = enter(Context::ArrayExpr);

        if (!context.stream().match(lexeme::leftBracket)) {
            return defaultError();
        }
        if (context.stream().match(lexeme::rightBracket)) {
            return syntax::expr::Array{{}, context.location()};
        }

        Vec<syntax::Node> elements;
        do {
            auto expr = handleExpr().value_or(syntax::RequiredButMissing{});
            context.emitIfNodeEmpty(expr, Reason::MissingExpr);
            elements.push_back(std::move(expr));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, Reason::MissingEnclosingSymbol
        );
        return syntax::expr::Array{std::move(elements), context.location()};
    }

    auto Parse::handleRecordExpr() -> ParseResult { // NOLINT(*-no-recursion)
        auto context = enter(Context::RecordExpr);

        auto type = handleTypeIdentifier().value_or({});
        if (context.backtrackIf(!context.stream().match(lexeme::leftBrace))) {
            return defaultError();
        }
        if (context.stream().match(lexeme::rightBrace)) {
            return syntax::expr::Record{
                std::move(type), {}, context.location()
            };
        }

        Vec<syntax::Node> entries;
        do {
            auto entryContext = enter(Context::RecordEntryExpr);
            entryContext.emitIfLexemeNotPresent(
                lexeme::identifier, Reason::MissingId
            );
            Str key = context.stream().current().str();

            entryContext.emitIfLexemeNotPresent(
                lexeme::colon, Reason::MissingSymbol
            );
            auto value = handleExpr().value_or(syntax::RequiredButMissing{});
            entryContext.emitIfNodeEmpty(value, Reason::MissingExpr);

            entries.emplace_back(syntax::expr::RecordEntry{
                std::move(key), std::move(value), entryContext.location()
            });
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, Reason::MissingEnclosingSymbol
        );
        return syntax::expr::Record{
            std::move(type), std::move(entries), context.location()
        };
    }

    auto Parse::handleString() -> ParseResult {
        auto context = enter(Context::StringExpr);

        Vec<Str> fragmentStrings;
        Vec<token::Token> placeholderTokens;
        while (context.stream().match(lexeme::stringFragment)) {
            fragmentStrings.push_back(context.stream().current().str());
            if (!context.stream().match(lexeme::stringPlaceholder)) {
                break;
            }
            placeholderTokens.push_back(context.stream().current());
        }

        if (fragmentStrings.empty()) {
            return defaultError();
        }
        if (fragmentStrings.size() == 1) {
            return syntax::expr::String{
                {fragmentStrings.front()}, {}, context.location()
            };
        }

        // prohibit recursive string interpolation
        context.emitIf(m_isSubroutine, Reason::RestrictedAction);

        Vec<syntax::Node> placeholders = placeholderTokens | rv::transform(
            [&context](token::Token const& token) {
                std::istringstream iss;
                iss.str(token.str());

                auto expr = Parse{
                    context.filepath(), lex::Lex::operator()(std::move(iss)),
                    Location{
                        .line = token.location().line,
                        .column = token.location().column -
                        context.location().column + 1
                    }
                }.handleExpr().value_or(syntax::RequiredButMissing{});
                context.emitIfNodeEmpty(expr, Reason::MissingExpr);
                return expr;
            }) | rng::to<Vec<syntax::Node>>();

        return syntax::expr::String{
            std::move(fragmentStrings), std::move(placeholders),
            context.location()
        };
    }

    auto Parse::handleTryExpr() -> ParseResult {
        auto context = enter(Context::TryExpr);

        if (!context.stream().match(lexeme::try_)) {
            return defaultError();
        }

        auto expr = handleExpr().value_or(syntax::RequiredButMissing{});
        context.emitIfNodeEmpty(expr, Reason::MissingExpr);
        return syntax::expr::Try{std::move(expr), context.location()};
    }
}
