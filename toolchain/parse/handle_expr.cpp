#include "parse.hpp"
#include "parse_unit.fwd.hpp"

#include "lex/lex.hpp"

namespace tlc::parse {
    auto handleExpr(Context context) -> Opt<syntax::Node> {
        Opt<syntax::Node> lhs;

        if (context.stream().match(syntax::isPrefixOperator)) {
            context.to(EContext::PrefixExpr);

            auto const op = context.stream().current().lexeme();
            lhs = handleExpr(Context::enter(EContext::Expr, context, syntax::opPrecedence(
                                                op, syntax::EOperator::Prefix)))
                .and_then([&](auto const& node) -> Opt<syntax::Node> {
                    return syntax::expr::Prefix{node, op, context.location()};
                });
        }
        else {
            lhs = handlePrimaryExpr(Context::enter(EContext::PrimaryExpr, context));
        }
        if (!lhs) {
            return {};
        }

        auto rhsContext = Context::enter(EContext::Expr, context);
        while (true) {
            if (syntax::isPostfixStart(rhsContext.stream().peek().lexeme())) {
                if (auto const tuple = handleTupleExpr(
                    Context::enter(EContext::TupleExpr, context)); tuple) {
                    lhs = syntax::expr::FnApp{
                        *lhs, *tuple, context.location()
                    };
                }
                else if (auto const array = handleArrayExpr(
                    Context::enter(EContext::ArrayExpr, context)); array) {
                    lhs = syntax::expr::Subscript{
                        *lhs, *array, context.location()
                    };
                }
            }
            else if (rhsContext.stream().match(syntax::isBinaryOperator)) {
                rhsContext.to(EContext::BinaryExpr);

                auto const op = rhsContext.stream().current().lexeme();
                auto const p = syntax::opPrecedence(
                    op, syntax::EOperator::Binary
                );
                if (rhsContext.backtrackIf(p <= context.minPrecedence())) {
                    break;
                }

                auto rhs = handleExpr(
                    Context::enter(EContext::Expr,
                                   context, syntax::isLeftAssociative(op) ? p + 1 : p)
                ).value_or(syntax::RequiredButMissing{});
                // rhsContext.emitIfNodeEmpty(rhs, EParseErrorReason::MissingExpr);
                lhs = syntax::expr::Binary{
                    *lhs, std::move(rhs), op, context.location()
                };
            }
            else {
                break;
            }
        }

        return lhs;
    }

    auto handlePrimaryExpr(Context context) -> Opt<syntax::Node> {
        if (auto const result = handleTryExpr(
            Context::enter(EContext::TryExpr, context)); result) {
            return result;
        }
        if (auto const result = handleSingleTokenLiteral(
            Context::enter(EContext::LiteralExpr, context)); result) {
            return result;
        }
        if (auto const result = handleRecordExpr(
            Context::enter(EContext::RecordExpr, context)); result) {
            return result;
        }
        if (auto const result = handleIdentifierLiteral(
            Context::enter(EContext::IdentifierExpr, context)); result) {
            return result;
        }
        if (auto const result = handleString(
            Context::enter(EContext::StringExpr, context)); result) {
            return result;
        }
        if (auto const result = handleTupleExpr(
            Context::enter(EContext::TupleExpr, context)); result) {
            return result;
        }
        if (auto const result = handleArrayExpr(
            Context::enter(EContext::ArrayExpr, context)); result) {
            return result;
        }
        return {};
    }

    auto handleSingleTokenLiteral(Context context) -> Opt<syntax::Node> {
        static const HashMap<lexeme::Lexeme, i32> baseTable = {
            {lexeme::integer2Literal, 2}, {lexeme::integer8Literal, 8},
            {lexeme::integer10Literal, 10}, {lexeme::integer16Literal, 16},
        };

        if (!context.stream().match(
            lexeme::integer2Literal, lexeme::integer8Literal,
            lexeme::integer10Literal, lexeme::integer16Literal,
            lexeme::floatLiteral, lexeme::true_, lexeme::false_
        )) {
            return {};
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

    auto handleIdentifierLiteral(Context context) -> Opt<syntax::Node> {
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
            return {};
        }

        return syntax::expr::Identifier{
            std::move(fragments), context.location()
        };
    }

    auto handleTupleExpr(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::leftParen)) {
            return {};
        }
        if (context.stream().match(lexeme::rightParen)) {
            return syntax::expr::Tuple{{}, context.location()};
        }

        Vec<syntax::Node> elements;
        do {
            auto expr = handleExpr(Context::enter(EContext::Expr, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(expr, EReason::MissingExpr);
            elements.push_back(std::move(expr));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightParen, EReason::MissingEnclosingSymbol
        );
        return syntax::expr::Tuple{std::move(elements), context.location()};
    }

    auto handleArrayExpr(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::leftBracket)) {
            return {};
        }
        if (context.stream().match(lexeme::rightBracket)) {
            return syntax::expr::Array{{}, context.location()};
        }

        Vec<syntax::Node> elements;
        do {
            auto expr = handleExpr(Context::enter(EContext::Expr, context))
                .value_or(syntax::RequiredButMissing{});
            context.emitIfNodeMissing(expr, EReason::MissingExpr);
            elements.push_back(std::move(expr));
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightBracket, EReason::MissingEnclosingSymbol
        );
        return syntax::expr::Array{std::move(elements), context.location()};
    }

    auto handleRecordExpr(Context context) -> Opt<syntax::Node> {
        auto type = handleTypeIdentifier(
            Context::enter(EContext::RecordExpr, context)
        ).value_or({});
        if (context.backtrackIf(!context.stream().match(lexeme::leftBrace))) {
            return {};
        }
        if (context.stream().match(lexeme::rightBrace)) {
            return syntax::expr::Record{
                std::move(type), {}, context.location()
            };
        }

        Vec<syntax::Node> entries;
        do {
            auto entryContext = Context::enter(EContext::RecordEntryExpr, context);
            entryContext.emitIfLexemeNotPresent(
                lexeme::identifier, EReason::MissingId
            );
            Str key = context.stream().current().str();

            entryContext.emitIfLexemeNotPresent(
                lexeme::colon, EReason::MissingSymbol
            );
            auto value = handleExpr(Context::enter(EContext::Expr, context))
                .value_or(syntax::RequiredButMissing{});
            entryContext.emitIfNodeMissing(value, EReason::MissingExpr);

            entries.emplace_back(syntax::expr::RecordEntry{
                std::move(key), std::move(value), entryContext.location()
            });
        }
        while (context.stream().match(lexeme::comma));

        context.emitIfLexemeNotPresent(
            lexeme::rightBrace, EReason::MissingEnclosingSymbol
        );
        return syntax::expr::Record{
            std::move(type), std::move(entries), context.location()
        };
    }

    auto handleString(Context context) -> Opt<syntax::Node> {
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
            return {};
        }
        if (fragmentStrings.size() == 1) {
            return syntax::expr::String{
                {fragmentStrings.front()}, {}, context.location()
            };
        }

        // prohibit recursive string interpolation
        context.emitIf(context.isSubroutine(), EReason::RestrictedAction);

        Vec<syntax::Node> placeholders = placeholderTokens | rv::transform(
            [&context](token::Token const& token) {
                std::istringstream iss;
                iss.str(token.str());

                auto parse = Parse{
                    context.filepath(), lex::Lex::operator()(std::move(iss)),
                    Location{
                        .line = token.location().line,
                        .column = token.location().column -
                        context.location().column + 1
                    }
                };
                auto expr = handleExpr(
                    parse.globalContext(EContext::Expr)
                ).value_or(syntax::RequiredButMissing{});
                context.emitIfNodeMissing(expr, EReason::MissingExpr);
                return expr;
            }) | rng::to<Vec<syntax::Node>>();

        return syntax::expr::String{
            std::move(fragmentStrings), std::move(placeholders),
            context.location()
        };
    }

    auto handleTryExpr(Context context) -> Opt<syntax::Node> {
        if (!context.stream().match(lexeme::try_)) {
            return {};
        }

        auto expr = handleExpr(Context::enter(EContext::Expr, context))
            .value_or(syntax::RequiredButMissing{});
        context.emitIfNodeMissing(expr, EReason::MissingExpr);
        return syntax::expr::Try{std::move(expr), context.location()};
    }
}
